/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 16:37:29 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 04:26:09 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "queue.h"
#include "lem_in.h"
#include <stdlib.h>
#include <limits.h>

struct s_link		*get_link(struct s_node *from, struct s_node *to)
{
	size_t	i;

	i = 0;
	while (i < from->links->len)
	{
		if (from->links->elems[i].ptr == to)
			return (&from->links->elems[i]);
		i++;
	}
	return (NULL);
}

bool	find_path(struct s_anthil *anthil)
{
	struct s_queue	*queue;
	struct s_node	**entry;
	struct s_node	*node;
	size_t			i;

	if (!(queue = create_queue(16)))
		return (false);
	if ((entry = queue_push(&queue)))
		*entry = &anthil->start->out;
	anthil->start->out.prev = &anthil->start->in;
	while (!queue_empty(queue) && (node = queue_pop(queue)) != &anthil->end->in)
	{
		i = -1;
		while (++i < node->links->len)
			if (!node->links->elems[i].ptr->prev
					&& node->links->elems[i].usable)
			{
				node->links->elems[i].ptr->prev = node;
				get_room(node->links->elems[i].ptr)->mark = true;
				if ((entry = queue_push(&queue)))
					*entry = node->links->elems[i].ptr;
			}
	}
	free(queue);
	return (anthil->end->in.prev != NULL);
}

bool	collect_path(struct s_anthil *anthil, struct s_room_vec **vec,
		struct s_node *node)
{
	size_t			i;
	struct s_room	**room;

	if (!(room = add_room(vec)))
		return (false);
	*room = anthil->start;
	if (!(room = add_room(vec)))
		return (false);
	*room = get_room(node);
	node = node->links->elems[0].ptr;
	while (node != &anthil->end->out)
	{
		i = -1;
		while (++i < node->links->len)
			if (!node->links->elems[i].virtual && !node->links->elems[i].usable)
			{
				node = node->links->elems[i].ptr;
				node = node->links->elems[0].ptr;
				if (!(room = add_room(vec)))
					return (false);
				*room = get_room(node);
				break ;
			}
	}
	return (true);
}

size_t	get_shortest_path(struct s_path_vec *paths)
{
	size_t	i;
	size_t	min;
	size_t	min_val;

	i = 0;
	min = 0;
	min_val = paths->paths[0].path->len;
	while (i < paths->len)
	{
		if (paths->paths[i].path->len < min_val)
		{
			min = i;
			min_val = paths->paths[i].path->len;
		}
		i++;
	}
	return (min);
}

size_t	distribute_ants(struct s_anthil *anthil, struct s_path_vec *paths)
{
	size_t	i;
	int32_t	diff;
	int32_t	ants;
	size_t	min;

	min = get_shortest_path(paths);
	ants = anthil->ants;
	while (ants)
	{
		i = 0;
		while (ants && i < paths->len)
		{
			diff = paths->paths[i].path->len - paths->paths[min].path->len;
			if (diff == 0 || paths->paths[min].ants > diff)
			{
				paths->paths[i].ants++;
				ants--;
			}
			i++;
		}
	}
	return (paths->paths[min].ants + paths->paths[min].path->len - 2);
}

static void	toggle_usability(struct s_anthil *anthil)
{
	struct s_link		*link;
	struct s_node		*node;

	node = &anthil->end->in;
	while (node != &anthil->start->in)
	{
		get_link(node->prev, node)->usable = false;
		if ((link = get_link(node, node->prev)))
			link->usable = true;
		else
		{
			if ((link = add_link(&node->links)))
			{
				link->ptr = node->prev;
				link->first = false;
				link->comments = NULL;
				link->virtual = true;
				link->usable = true;
			}
		}
		node = node->prev;
	}
}

struct s_path_vec	*get_paths(struct s_anthil *anthil)
{
	struct s_path_vec	*paths;
	struct s_path		*path;
	struct s_room_vec	*current;
	size_t				i;

	if (!(paths = create_path_vec(10)))
		return (NULL);
	i = -1;
	while (++i < anthil->start->out.links->len)
		if (!anthil->start->out.links->elems[i].usable)
		{
			if (!(current = create_room_vec(10)))
				return (free_paths(paths));
			if (!collect_path(anthil, &current,
						anthil->start->out.links->elems[i].ptr))
				return ((void*)(uint64_t)(free_paths(paths) || ffree(current)));
			if ((path = add_path(&paths)))
				*path = (struct s_path) {
					.ants = 0,
					.path = current
				};
			else
				return (free_paths(paths));
		}
	return (paths);
}

void	find_all_paths(struct s_anthil *anthil)
{
	struct s_path_vec	*paths;
	size_t				last_max;
	size_t				max;

	last_max = SIZE_MAX;
	while (find_path(anthil))
	{
		toggle_usability(anthil);
		if (!(paths = get_paths(anthil)))
			break ;
		if ((max = distribute_ants(anthil, paths)) < last_max)
		{
			last_max = max;
			if (anthil->paths)
				free_paths(anthil->paths);
			anthil->paths = paths;
		}
		else
		{
			free_paths(paths);
			break ;
		}
		unmark(anthil->start);
	}
}
