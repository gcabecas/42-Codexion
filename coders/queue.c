/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 10:33:26 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/31 11:01:28 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	queue_init(t_queue *h, int cap)
{
	h->data = malloc(sizeof(t_request) * cap);
	if (!h->data)
		return (0);
	h->size = 0;
	h->capacity = cap;
	return (1);
}

void	queue_destroy(t_queue *h)
{
	free(h->data);
	h->data = NULL;
	h->size = 0;
	h->capacity = 0;
}

void	queue_push(t_queue *h, t_request req)
{
	if (h->size >= h->capacity)
		return ;
	h->data[h->size] = req;
	queue_sift_up(h, h->size);
	h->size++;
}

t_request	queue_pop(t_queue *h)
{
	t_request	top;

	top = h->data[0];
	h->size--;
	if (h->size > 0)
	{
		h->data[0] = h->data[h->size];
		queue_sift_down(h, 0);
	}
	return (top);
}

t_request	*queue_peek(t_queue *h)
{
	if (h->size == 0)
		return (NULL);
	return (&h->data[0]);
}
