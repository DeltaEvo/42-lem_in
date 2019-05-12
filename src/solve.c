/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 16:37:29 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/12 14:28:53 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "queue.h"
#include "lem_in.h"
#include <stdlib.h>
#include <limits.h>

void	unmark(struct s_room *room);

struct s_room	*find_path(struct s_anthil *anthil, size_t max_depth)
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
		if (room == anthil->end)
			break ;
		/*if (room->depth >= max_depth)
			continue ;*/
		if (!room->old_prev || room->broken)
		{
			i = 0;
			while (i < room->links->len)
			{
				if (!room->links->rooms[i].ptr->mark
					&& room->links->rooms[i].ptr->old_prev != room)
				{
					room->links->rooms[i].ptr->mark = true;
					room->links->rooms[i].ptr->prev = room;
					room->links->rooms[i].ptr->depth = room->depth + 1;
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
	fprintf(stderr, "Last depth %zu max %zu\n", room->depth, max_depth);
	if (room == anthil->end)
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
	if (room->old_prev)
		collect_path(anthil, vec, room->old_prev);
	add_room(vec)->ptr = room;
}

size_t	distribute_ants(struct s_anthil *anthil)
{
	size_t	i;
	size_t	diff;
	size_t	ants;
	size_t	min;
	size_t	min_val;

	i = 0;
	min = 0;
	min_val = anthil->paths->paths[0].path->len;
	while (i < anthil->paths->len)
	{
		if (anthil->paths->paths[i].path->len < min_val)
		{
			min = i;
			min_val = anthil->paths->paths[i].path->len;
		}
		anthil->paths->paths[i].ants = 0;
		i++;
	}
	//fprintf(stderr, "Min %zu with %zu len\n", min, min_val);
	ants = anthil->ants;
	while (ants)
	{
		i = 0;
		while (ants && i < anthil->paths->len)
		{
			diff = anthil->paths->paths[i].path->len - anthil->paths->paths[min].path->len;
			if (diff == 0 || anthil->paths->paths[min].ants > diff)
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
		if (anthil->paths->paths[i].ants)
			fprintf(stderr, "Ants in %zu: %zu (turns: %zu)\n", i, anthil->paths->paths[i].ants, anthil->paths->paths[i].ants + anthil->paths->paths[i].path->len - 2);
		i++;
	}
	return (anthil->paths->paths[min].ants + anthil->paths->paths[min].path->len - 2);
}

void	find_all_paths(struct s_anthil *anthil)
{
	struct s_room		*room;
	struct s_room_vec	*current;
	struct s_path		*path;
	size_t				i;
	size_t				max_depth;

	fprintf(stderr, "Ants: %zu\n", anthil->ants);
	max_depth = SIZE_MAX;
	while ((room = find_path(anthil, max_depth)))
	{
		while (room != anthil->start)
		{
			if (!room->broken)
				room->old_prev = room->prev;
			room = room->prev;
		}
		i = 0;
		anthil->paths = create_path_vec(10);
		while (i < anthil->end->links->len)
		{
			if (anthil->end->links->rooms[i].ptr->old_prev)
			{
				current = create_room_vec(10);
				collect_path(anthil, &current, anthil->end->links->rooms[i].ptr);
				add_room(&current)->ptr = anthil->end;
				print_path(current);
				if ((path = add_path(&anthil->paths)))
					*path = (struct s_path) {
						.ants = 0,
						.path = current
					};
			}
			i++;
		}
		max_depth = distribute_ants(anthil);
		unmark(anthil->start);
	}
}
