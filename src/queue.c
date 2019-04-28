/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 14:05:33 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/04/28 15:17:43 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "queue.h"
#include "mem.h"
#include <stdlib.h>

struct s_queue 	*create_queue(size_t size)
{
	struct s_queue	*queue;

	if (!(queue = malloc(sizeof(*queue) + size * sizeof(*queue->rooms))))
		return (NULL);
	*queue = (struct s_queue) {
		.head = 0,
		.tail = 0,
		.full = false,
		.size = size
	};
	return (queue);
}

bool			queue_empty(struct s_queue *queue)
{
	return (!queue->full && queue->head == queue->tail);
}

struct s_room	**queue_push(struct s_queue **queue)
{
	size_t			new_size;
	struct s_room	**el;

	if ((*queue)->full)
	{
		new_size = (*queue)->size * 2;
		*queue = ft_realloc(*queue,
				sizeof(**queue) + (*queue)->size * sizeof(*(*queue)->rooms),
				sizeof(**queue) + new_size * sizeof(*(*queue)->rooms));
		if (!*queue)
			return (NULL);
		(*queue)->size = new_size;
	}
	el = &(*queue)->rooms[(*queue)->head];
	(*queue)->head = ((*queue)->head + 1) % (*queue)->size;
	(*queue)->full = (*queue)->head == (*queue)->tail;
	return (el);
}

struct s_room 	*queue_pop(struct s_queue *queue) {
	struct s_room	*el;

	if (queue_empty(queue))
		return (NULL);
	el = queue->rooms[queue->tail];
	queue->full = false;
	queue->tail = (queue->tail + 1) % queue->size;
	return (el);
}
