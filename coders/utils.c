/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:07:02 by gcabecas          #+#    #+#             */
/*   Updated: 2026/04/03 16:48:46 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time_ms(t_sim *sim)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000 - sim->start_time);
}

void	print_log(t_sim *sim, int id, char *msg)
{
	long long	ts;
	int			allow_log;

	pthread_mutex_lock(&sim->print_mutex);
	pthread_mutex_lock(&sim->stop_mutex);
	allow_log = (!sim->stop || strcmp(msg, "burned out") == 0);
	pthread_mutex_unlock(&sim->stop_mutex);
	if (allow_log)
	{
		ts = get_time_ms(sim);
		printf("%lld %d %s\n", ts, id, msg);
	}
	pthread_mutex_unlock(&sim->print_mutex);
}

void	set_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
}

int	is_stopped(t_sim *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->stop_mutex);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stopped);
}
