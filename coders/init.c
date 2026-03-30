/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:43:28 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/30 12:58:04 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_sim	*ft_alloc_sim(t_args *args)
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
	if (!sim->coders || !sim->threads)
	{
		free(sim->coders);
		free(sim->threads);
		free(sim);
		return (NULL);
	}
	return (sim);
}

t_sim	*init_sim(t_args *args)
{
	t_sim	*sim;
	int		i;

	sim = ft_alloc_sim(args);
	if (!sim)
		return (NULL);
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
	{
		free(sim->coders);
		free(sim->threads);
		free(sim);
		return (NULL);
	}
	i = 0;
	while (i < args->nb_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].sim = sim;
		i++;
	}
	return (sim);
}

void	free_sim(t_sim *sim)
{
	if (!sim)
		return ;
	free(sim->coders);
	free(sim->threads);
	pthread_mutex_destroy(&sim->print_mutex);
	free(sim);
}
