/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:48:03 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/31 12:39:51 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	join_some_threads(t_sim *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(sim->threads[i], NULL);
		i++;
	}
}

static void	run_cycle(t_coder *coder)
{
	if (!take_dongles(coder))
		return ;
	set_compile_start(coder, get_time_ms(coder->sim));
	print_log(coder->sim, coder->id, "is compiling");
	usleep((unsigned int)coder->sim->args->time_to_compile * 1000);
	release_dongles(coder);
	inc_compile_count(coder);
	if (is_stopped(coder->sim))
		return ;
	print_log(coder->sim, coder->id, "is debugging");
	usleep((unsigned int)coder->sim->args->time_to_debug * 1000);
	if (is_stopped(coder->sim))
		return ;
	print_log(coder->sim, coder->id, "is refactoring");
	usleep((unsigned int)coder->sim->args->time_to_refactor * 1000);
}

static void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (!wait_sim_start(coder->sim))
		return (NULL);
	while (!is_stopped(coder->sim))
	{
		run_cycle(coder);
	}
	return (NULL);
}

static int	create_coder_threads(t_sim *sim)
{
	int	i;
	int	ret;

	i = 0;
	while (i < sim->args->nb_coders)
	{
		ret = pthread_create(&sim->threads[i], NULL,
				coder_routine, &sim->coders[i]);
		if (ret != 0)
		{
			set_stop(sim);
			start_sim(sim);
			join_some_threads(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	create_threads(t_sim *sim)
{
	if (!create_coder_threads(sim))
		return (0);
	if (pthread_create(&sim->burnout, NULL, burnout_routine, sim) != 0)
	{
		set_stop(sim);
		start_sim(sim);
		join_some_threads(sim, sim->args->nb_coders);
		return (0);
	}
	start_sim(sim);
	return (1);
}
