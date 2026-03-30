/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:48:03 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/30 12:58:07 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->sim->print_mutex);
	printf("coder %d started\n", coder->id);
	pthread_mutex_unlock(&coder->sim->print_mutex);
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
