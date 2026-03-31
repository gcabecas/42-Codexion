/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 10:42:27 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/31 12:39:50 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	queue_sift_up(t_queue *h, int idx)
{
	int			parent;
	t_request	tmp;

	while (idx > 0)
	{
		parent = (idx - 1) / 2;
		if (h->data[parent].priority <= h->data[idx].priority)
			break ;
		tmp = h->data[parent];
		h->data[parent] = h->data[idx];
		h->data[idx] = tmp;
		idx = parent;
	}
}

static int	get_smallest(t_queue *h, int idx)
{
	int	smallest;
	int	left;
	int	right;

	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;
	if (left < h->size
		&& h->data[left].priority < h->data[smallest].priority)
		smallest = left;
	if (right < h->size
		&& h->data[right].priority < h->data[smallest].priority)
		smallest = right;
	return (smallest);
}

void	queue_sift_down(t_queue *h, int idx)
{
	int			smallest;
	t_request	tmp;

	while (1)
	{
		smallest = get_smallest(h, idx);
		if (smallest == idx)
			break ;
		tmp = h->data[idx];
		h->data[idx] = h->data[smallest];
		h->data[smallest] = tmp;
		idx = smallest;
	}
}
