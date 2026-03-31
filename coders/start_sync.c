/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_sync.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:42:20 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/31 12:43:08 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	wait_sim_start(t_sim *sim)
{
	pthread_mutex_lock(&sim->start_mutex);
	while (!sim->start_ready && !sim->stop)
		pthread_cond_wait(&sim->start_cond, &sim->start_mutex);
	pthread_mutex_unlock(&sim->start_mutex);
	return (!is_stopped(sim));
}

void	start_sim(t_sim *sim)
{
	pthread_mutex_lock(&sim->start_mutex);
	sim->start_ready = 1;
	pthread_cond_broadcast(&sim->start_cond);
	pthread_mutex_unlock(&sim->start_mutex);
}
