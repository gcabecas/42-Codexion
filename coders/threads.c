/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:48:03 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/30 13:06:45 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_all_done(t_sim *sim)
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

static void	ft_run_cycle(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	coder->last_compile_start = get_time_ms(sim);
	print_log(sim, coder->id, "is compiling");
	usleep((unsigned int)sim->args->time_to_compile * 1000);
	coder->compile_count++;
	print_log(sim, coder->id, "is debugging");
	usleep((unsigned int)sim->args->time_to_debug * 1000);
	print_log(sim, coder->id, "is refactoring");
	usleep((unsigned int)sim->args->time_to_refactor * 1000);
}

static void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!coder->sim->stop)
	{
		ft_run_cycle(coder);
		if (ft_all_done(coder->sim))
		{
			coder->sim->stop = 1;
			break ;
		}
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
			sim->stop = 1;
			return (0);
		}
		i++;
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
}
