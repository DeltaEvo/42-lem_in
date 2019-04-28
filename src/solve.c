/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 16:37:29 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/01 10:20:10 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "queue.h"
#include "lem_in.h"
#include <stdlib.h>

void	unmark(struct s_room *room);

struct s_room	*find_path(struct s_anthil *anthil)
{
	struct s_queue	*queue;
	struct s_room	*room;
	size_t			i;

	queue = create_queue(1);
	*queue_push(&queue) = anthil->start;
	anthil->start->mark = true;
	room = NULL;
	while (!queue_empty(queue))
	{
		room = queue_pop(queue);
		if (room->end)
			break ;
		if (!room->old_prev || room->old_prev->broken)
		{
			i = 0;
			while (i < room->links->len)
			{
				if (!room->links->rooms[i].ptr->mark
					&& room->links->rooms[i].ptr->old_prev != room)
				{
					room->links->rooms[i].ptr->mark = true;
					room->links->rooms[i].ptr->prev = room;
					*queue_push(&queue) = room->links->rooms[i].ptr;
				}
				i++;
			}
		}
		else if (!room->old_prev->mark)
		{
			room->old_prev->prev = room;
			room->old_prev->mark = true;
			room->old_prev->broken = true;
			*queue_push(&queue) = room->old_prev;
		}
	}
	free(queue);
	if (room && room->end)
		return (room);
	return (NULL);
}

void	print_path(struct s_room_vec *vec)
{
	size_t	i;
	
	i = 0;
	while (i < vec->len)
	{
		fprintf(stderr, "%s->", vec->rooms[i].ptr->name);
		i++;
	}
	fprintf(stderr, "\n");
	fprintf(stderr, "Send: %zu\n", vec->len);
}

void	collect_path(struct s_anthil *anthil, struct s_room_vec **vec, struct s_room *room)
{
	if (room->prev)
		collect_path(anthil, vec, room->prev);
	add_room(vec)->ptr = room;
}

void	distribute_ants(struct s_anthil *anthil)
{
	size_t	i;
	size_t	diff;
	size_t	ants;

	i = 0;
	while (i < anthil->paths->len)
	{
		anthil->paths->paths[i].ants = 0;
		i++;
	}
	ants = anthil->ants;
	while (ants)
	{
		i = 0;
		while (ants && i < anthil->paths->len)
		{
			diff = anthil->paths->paths[i].path->len - anthil->paths->paths[0].path->len;
			if (anthil->paths->paths[0].ants >= diff)
			{
				anthil->paths->paths[i].ants++;
				ants--;
			}
			i++;
		}
	}
	i = 0;
	while (i < anthil->paths->len)
	{
		fprintf(stderr, "Ants in %zu: %zu (turns: %zu)\n", i, anthil->paths->paths[i].ants, anthil->paths->paths[i].ants + anthil->paths->paths[i].path->len - 2);
		i++;
	}
}

void	find_all_paths(struct s_anthil *anthil)
{
	struct s_room		*room;
	struct s_room_vec	*current;
	struct s_path		*path;

	fprintf(stderr, "Ants: %zu\n", anthil->ants);
	while ((room = find_path(anthil)))
	{
		if (!anthil->paths)
			anthil->paths = create_path_vec(10);
		current = create_room_vec(10);
		collect_path(anthil, &current, room);
		print_path(current);
		if ((path = add_path(&anthil->paths)))
			*path = (struct s_path) {
				.ants = 0,
				.path = current
			};
		distribute_ants(anthil);
		if (!path->ants)
			break ;
		while (room != anthil->start)
		{
			if (!room->broken)
				room->old_prev = room->prev;
			room = room->prev;
		}
		unmark(anthil->start);
	}
}
