/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:48:14 by gcabecas          #+#    #+#             */
/*   Updated: 2026/04/03 15:56:10 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	can_take(t_dongle *d, t_sim *sim, int coder_id)
{
	t_request	*top;
	long long	elapsed;

	if (d->held)
		return (0);
	if (d->last_release)
	{
		elapsed = get_time_ms(sim) - d->last_release;
		if (elapsed < sim->args->dongle_cooldown)
			return (0);
	}
	top = queue_peek(&d->queue);
	if (!top || top->coder_id != coder_id)
		return (0);
	return (1);
}

static long long	get_priority(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	if (sim->args->is_edf)
		return (get_compile_start(coder) + sim->args->time_to_burnout);
	return (get_time_ms(sim));
}

static int	wait_dongle(t_coder *coder, int idx)
{
	t_dongle		*d;
	t_request		req;

	d = &coder->sim->dongles[idx];
	pthread_mutex_lock(&d->mutex);
	req.coder_id = coder->id;
	req.priority = get_priority(coder);
	queue_push(&d->queue, req);
	while (!is_stopped(coder->sim) && !can_take(d, coder->sim, coder->id))
	{
		pthread_mutex_unlock(&d->mutex);
		usleep(1000);
		pthread_mutex_lock(&d->mutex);
	}
	if (is_stopped(coder->sim))
	{
		pthread_mutex_unlock(&d->mutex);
		return (0);
	}
	d->held = 1;
	queue_pop(&d->queue);
	pthread_mutex_unlock(&d->mutex);
	return (1);
}

int	take_dongles(t_coder *coder)
{
	int	first;
	int	second;

	first = coder->id - 1;
	second = coder->id % coder->sim->args->nb_coders;
	if (first > second)
	{
		first = coder->id % coder->sim->args->nb_coders;
		second = coder->id - 1;
	}
	if (first == second)
	{
		usleep(1000);
		return (0);
	}
	if (!wait_dongle(coder, first))
		return (0);
	print_log(coder->sim, coder->id, "has taken a dongle");
	if (!wait_dongle(coder, second))
	{
		release_one(coder, first);
		return (0);
	}
	print_log(coder->sim, coder->id, "has taken a dongle");
	return (1);
}
