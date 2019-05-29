/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 11:40:01 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/28 23:31:57 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

struct s_room	*get_room(struct s_node *node)
{
	if (node->in)
		return ((struct s_room *)((char *)node
					- (char *)&((struct s_room *)0)->in));
	else
		return ((struct s_room *)((char *)node
					- (char *)&((struct s_room *)0)->out));
}

void			unmark(struct s_room *room)
{
	size_t	i;

	room->mark = false;
	room->out.prev = NULL;
	room->in.prev = NULL;
	i = 0;
	while (i < room->out.links->len)
	{
		if (get_room(room->out.links->elems[i].ptr)->mark)
			unmark(get_room(room->out.links->elems[i].ptr));
		i++;
	}
}
