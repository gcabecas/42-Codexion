/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:48:14 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/31 09:28:43 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	dongle_available(t_sim *sim, int idx)
{
	long long	released;

	released = sim->dongles[idx].last_release;
	if (!released)
		return (1);
	return (get_time_ms(sim) - released >= sim->args->dongle_cooldown);
}

static int	try_take_dongle(t_coder *coder, int idx)
{
	t_sim	*sim;

	sim = coder->sim;
	if (pthread_mutex_trylock(&sim->dongles[idx].mutex) != 0)
		return (0);
	if (!dongle_available(sim, idx))
	{
		pthread_mutex_unlock(&sim->dongles[idx].mutex);
		return (0);
	}
	return (1);
}

static int	acquire_first(t_coder *coder, int first, int second)
{
	t_sim	*sim;

	sim = coder->sim;
	while (!is_stopped(sim))
	{
		if (try_take_dongle(coder, first))
		{
			print_log(sim, coder->id, "has taken a dongle");
			if (first != second)
				return (1);
			while (!is_stopped(sim))
				usleep(100);
			pthread_mutex_unlock(&sim->dongles[first].mutex);
			return (0);
		}
		usleep(500);
	}
	return (0);
}

int	take_dongles(t_coder *coder)
{
	t_sim	*sim;
	int		first;
	int		second;

	sim = coder->sim;
	first = coder->id - 1;
	second = coder->id % sim->args->nb_coders;
	if (first > second)
	{
		first = coder->id % sim->args->nb_coders;
		second = coder->id - 1;
	}
	if (!acquire_first(coder, first, second))
		return (0);
	while (!is_stopped(sim))
	{
		if (try_take_dongle(coder, second))
		{
			print_log(sim, coder->id, "has taken a dongle");
			return (1);
		}
		usleep(500);
	}
	pthread_mutex_unlock(&sim->dongles[first].mutex);
	return (0);
}

void	release_dongles(t_coder *coder)
{
	t_sim		*sim;
	int			left;
	int			right;
	long long	now;

	sim = coder->sim;
	left = coder->id - 1;
	right = coder->id % sim->args->nb_coders;
	now = get_time_ms(sim);
	sim->dongles[left].last_release = now;
	pthread_mutex_unlock(&sim->dongles[left].mutex);
	if (left == right)
		return ;
	sim->dongles[right].last_release = now;
	pthread_mutex_unlock(&sim->dongles[right].mutex);
}
