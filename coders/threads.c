/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:48:03 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/31 09:29:01 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	run_cycle(t_coder *coder)
{
	if (!take_dongles(coder))
		return ;
	coder->last_compile_start = get_time_ms(coder->sim);
	print_log(coder->sim, coder->id, "is compiling");
	usleep((unsigned int)coder->sim->args->time_to_compile * 1000);
	release_dongles(coder);
	coder->compile_count++;
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
	while (!is_stopped(coder->sim))
	{
		run_cycle(coder);
	}
	return (NULL);
}

int	create_threads(t_sim *sim)
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
			return (0);
		}
		i++;
	}
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
	{
		set_stop(sim);
		return (0);
	}
	return (1);
}

void	join_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->nb_coders)
	{
		pthread_join(sim->threads[i], NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
}
