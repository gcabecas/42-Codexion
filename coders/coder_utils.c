/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 10:57:00 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/31 12:39:39 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_compile_start(t_coder *coder, long long time)
{
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = time;
	pthread_mutex_unlock(&coder->mutex);
}

long long	get_compile_start(t_coder *coder)
{
	long long	time;

	pthread_mutex_lock(&coder->mutex);
	time = coder->last_compile_start;
	pthread_mutex_unlock(&coder->mutex);
	return (time);
}

void	inc_compile_count(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->mutex);
}

int	get_compile_count(t_coder *coder)
{
	int	count;

	pthread_mutex_lock(&coder->mutex);
	count = coder->compile_count;
	pthread_mutex_unlock(&coder->mutex);
	return (count);
}
