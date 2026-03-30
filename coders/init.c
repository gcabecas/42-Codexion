/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:43:28 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/30 21:36:48 by gcabecas         ###   ########lyon.fr   */
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
		pthread_mutex_destroy(&sim->dongles[i++].mutex);
	pthread_mutex_destroy(&sim->print_mutex);
	free(sim->coders);
	free(sim->threads);
	free(sim->dongles);
	free(sim);
	return (NULL);
}

t_sim	*init_sim(t_args *args)
{
	t_sim	*sim;
	int		i;

	sim = alloc_sim(args);
	if (!sim)
		return (NULL);
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
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
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (cleanup(sim, i));
		sim->dongles[i].last_release = 0;
		sim->coders[i].id = i + 1;
		sim->coders[i].sim = sim;
		i++;
	}
	return (sim);
}

void	free_sim(t_sim *sim)
{
	int	i;

	if (!sim)
		return ;
	i = 0;
	while (i < sim->args->nb_coders)
		pthread_mutex_destroy(&sim->dongles[i++].mutex);
	free(sim->dongles);
	free(sim->coders);
	free(sim->threads);
	pthread_mutex_destroy(&sim->print_mutex);
	free(sim);
}
