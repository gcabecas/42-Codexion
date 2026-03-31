/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 10:42:52 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/31 11:01:19 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_sim(t_sim *sim)
{
	int	i;

	if (!sim)
		return ;
	i = 0;
	while (i < sim->args->nb_coders)
	{
		pthread_cond_destroy(&sim->dongles[i].cond);
		queue_destroy(&sim->dongles[i].queue);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_mutex_destroy(&sim->coders[i].mutex);
		i++;
	}
	free(sim->dongles);
	free(sim->coders);
	free(sim->threads);
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
	free(sim);
}
