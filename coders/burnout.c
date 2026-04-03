/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   burnout.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 09:05:48 by gcabecas          #+#    #+#             */
/*   Updated: 2026/04/03 16:48:46 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	all_done(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->nb_coders)
	{
		if (get_compile_count(&sim->coders[i]) < sim->args->nb_compiles)
			return (0);
		i++;
	}
	return (1);
}

static void	check_burnout(t_sim *sim)
{
	int			i;
	long long	elapsed;

	i = 0;
	while (i < sim->args->nb_coders)
	{
		elapsed = get_time_ms(sim) - get_compile_start(&sim->coders[i]);
		if (elapsed > sim->args->time_to_burnout)
		{
			set_stop(sim);
			print_log(sim, sim->coders[i].id, "burned out");
			return ;
		}
		i++;
	}
}

void	*burnout_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	if (!wait_sim_start(sim))
		return (NULL);
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
