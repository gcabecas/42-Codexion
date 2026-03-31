/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_join.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:42:12 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/31 12:43:10 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	join_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->nb_coders)
	{
		pthread_join(sim->threads[i], NULL);
		i++;
	}
	pthread_join(sim->burnout, NULL);
}
