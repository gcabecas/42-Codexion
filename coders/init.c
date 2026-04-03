/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:43:28 by gcabecas          #+#    #+#             */
/*   Updated: 2026/04/03 15:51:44 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_sim	*alloc_sim(t_args *args)
{
	t_sim			*sim;
	struct timeval	tv;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (NULL);
	memset(sim, 0, sizeof(t_sim));
	sim->args = args;
	gettimeofday(&tv, NULL);
	sim->start_time = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
	sim->coders = malloc(sizeof(t_coder) * args->nb_coders);
	sim->threads = malloc(sizeof(pthread_t) * args->nb_coders);
	sim->dongles = malloc(sizeof(t_dongle) * args->nb_coders);
	if (!sim->coders || !sim->threads || !sim->dongles)
	{
		free(sim->coders);
		free(sim->threads);
		free(sim->dongles);
		free(sim);
		return (NULL);
	}
	return (sim);
}

static void	*cleanup(t_sim *sim, int n_dongles)
{
	int	i;

	i = 0;
	while (i < n_dongles)
	{
		pthread_cond_destroy(&sim->dongles[i].cond);
		queue_destroy(&sim->dongles[i].queue);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_mutex_destroy(&sim->coders[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_cond_destroy(&sim->start_cond);
	pthread_mutex_destroy(&sim->start_mutex);
	free(sim->coders);
	free(sim->threads);
	free(sim->dongles);
	free(sim);
	return (NULL);
}

static int	init_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->stop_mutex);
		return (0);
	}
	if (pthread_mutex_init(&sim->start_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_mutex);
		pthread_mutex_destroy(&sim->stop_mutex);
		return (0);
	}
	if (pthread_cond_init(&sim->start_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->start_mutex);
		pthread_mutex_destroy(&sim->print_mutex);
		pthread_mutex_destroy(&sim->stop_mutex);
		return (0);
	}
	return (1);
}

static int	init_entity(t_sim *sim, int i)
{
	if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		return (0);
	}
	if (!queue_init(&sim->dongles[i].queue, sim->args->nb_coders))
	{
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		return (0);
	}
	sim->dongles[i].last_release = 0;
	sim->dongles[i].held = 0;
	if (pthread_mutex_init(&sim->coders[i].mutex, NULL) != 0)
	{
		queue_destroy(&sim->dongles[i].queue);
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		return (0);
	}
	sim->coders[i].id = i + 1;
	sim->coders[i].compile_count = 0;
	sim->coders[i].last_compile_start = 0;
	sim->coders[i].sim = sim;
	return (1);
}

t_sim	*init_sim(t_args *args)
{
	t_sim	*sim;
	int		i;

	sim = alloc_sim(args);
	if (!sim)
		return (NULL);
	if (!init_mutexes(sim))
	{
		free(sim->coders);
		free(sim->threads);
		free(sim->dongles);
		free(sim);
		return (NULL);
	}
	i = 0;
	while (i < args->nb_coders)
	{
		if (!init_entity(sim, i))
			return (cleanup(sim, i));
		i++;
	}
	return (sim);
}
