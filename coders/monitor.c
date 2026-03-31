/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 09:05:48 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/31 09:28:57 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	all_done(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->nb_coders)
	{
		if (sim->coders[i].compile_count < sim->args->nb_compiles)
			return (0);
		i++;
	}
	return (1);
}

static void	check_burnout(t_sim *sim)
{
	int			i;
	long long	now;
	long long	elapsed;

	now = get_time_ms(sim);
	i = 0;
	while (i < sim->args->nb_coders)
	{
		elapsed = now - sim->coders[i].last_compile_start;
		if (elapsed > sim->args->time_to_burnout)
		{
			print_log(sim, sim->coders[i].id, "burned out");
			set_stop(sim);
			return ;
		}
		i++;
	}
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!is_stopped(sim))
	{
		check_burnout(sim);
		if (is_stopped(sim) || all_done(sim))
		{
			set_stop(sim);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}
