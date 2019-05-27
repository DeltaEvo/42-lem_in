/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 16:37:29 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/27 12:13:52 by dde-jesu         ###   ########.fr       */
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

#include <assert.h>

bool	find_path(struct s_anthil *anthil)
{
	struct s_queue 	*queue;
	struct s_node	*node;
	size_t			i;

	queue = create_queue(16);
	*queue_push(&queue) = &anthil->start->out;
	anthil->start->out.prev = &anthil->start->in;
	while (!queue_empty(queue) && (node = queue_pop(queue)) != &anthil->end->in)
	{
		i = 0;
		while (i < node->links->len)
		{
			if (!node->links->elems[i].ptr->prev
					&& node->links->elems[i].usable)
			{
				node->links->elems[i].ptr->prev = node;
				get_room(node->links->elems[i].ptr)->mark = true;
				*queue_push(&queue) = node->links->elems[i].ptr;
			}
			i++;
		}
	}
	free(queue);
	return (anthil->end->in.prev != NULL);
}

void	collect_path(struct s_anthil *anthil, struct s_room_vec **vec,
		struct s_node *node)
{
	size_t	i;

	assert(node->in);
	*add_room(vec) = anthil->start;
	*add_room(vec) = get_room(node);
	node = node->links->elems[0].ptr;
	while (node != &anthil->end->out)
	{
		i = 0;
		while (i < node->links->len)
		{
			if (!node->links->elems[i].virtual && !node->links->elems[i].usable)
			{
				node = node->links->elems[i].ptr;
				assert(node->in);
				node = node->links->elems[0].ptr;
				assert(!node->in);
				*add_room(vec) = get_room(node);
				break ;
			}
			i++;
			assert(i < node->links->len);
		}
	}
}

size_t	distribute_ants(struct s_anthil *anthil, struct s_path_vec *paths)
{
	size_t	i;
	size_t	diff;
	int32_t	ants;
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
		paths->paths[i].ants = 0;
		i++;
	}
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

	paths = create_path_vec(10);
	i = 0;
	while (i < anthil->start->out.links->len)
	{
		if (!anthil->start->out.links->elems[i].usable)
		{
			current = create_room_vec(10);
			collect_path(anthil, &current,
					anthil->start->out.links->elems[i].ptr);
			if ((path = add_path(&paths)))
				*path = (struct s_path) {
					.ants = 0,
					.path = current
				};
			else
			{
				// TODO
			}
		}
		i++;
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
		paths = get_paths(anthil);
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
