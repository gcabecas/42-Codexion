/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_release.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 10:55:41 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/31 12:39:44 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_one(t_coder *coder, int idx)
{
	t_sim		*sim;
	t_dongle	*d;

	sim = coder->sim;
	d = &sim->dongles[idx];
	pthread_mutex_lock(&d->mutex);
	d->held = 0;
	d->last_release = get_time_ms(sim);
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}

void	release_dongles(t_coder *coder)
{
	t_sim	*sim;
	int		left;
	int		right;

	sim = coder->sim;
	left = coder->id - 1;
	right = coder->id % sim->args->nb_coders;
	release_one(coder, left);
	if (left != right)
		release_one(coder, right);
}
