/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 16:37:29 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/27 11:14:01 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "queue.h"
#include "lem_in.h"
#include <stdlib.h>
#include <limits.h>

void			unmark(struct s_room *room);
struct s_room	*get_room(struct s_node *node);

struct s_link       *get_link(struct s_node *from, struct s_node *to)
{
	size_t  i;

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

bool find_path(struct s_anthil *anthil)
{
	struct s_queue	*queue;
	struct s_node	*node;
	size_t			i;

	queue = create_queue(1);
	*queue_push(&queue) = &anthil->start->out;
	anthil->start->out.prev = &anthil->start->in;
	node = NULL;
	while (!queue_empty(queue))
	{
		node = queue_pop(queue);
		if (node == &anthil->end->in)
			break ;
		i = 0;
		while (i < node->links->len)
		{
			if (!node->links->elems[i].ptr->prev && node->links->elems[i].usable)
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

void	print_path(struct s_room_vec *vec)
{
	size_t	i;
	
	i = 0;
	while (i < vec->len)
	{
		fprintf(stderr, "%s->", vec->rooms[i]->name);
		i++;
	}
	fprintf(stderr, "\n");
	fprintf(stderr, "Send: %zu\n", vec->len);
}

void	collect_path(struct s_anthil *anthil, struct s_room_vec **vec, struct s_node *node)
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
	size_t	ants;
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
	//fprintf(stderr, "Min %zu with %zu len\n", min, min_val);
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
	i = 0;
	while (i < paths->len)
	{
		if (paths->paths[i].ants)
			fprintf(stderr, "Ants in %zu: %zu (turns: %zu)\n", i, paths->paths[i].ants, paths->paths[i].ants + paths->paths[i].path->len - 2);
		i++;
	}
	return (paths->paths[min].ants + paths->paths[min].path->len - 2);
}

void	find_all_paths(struct s_anthil *anthil)
{
	struct s_node		*node;
	struct s_link		*link;
	struct s_room_vec	*current;
	struct s_path_vec	*paths;
	struct s_path		*path;
	size_t				i;
	size_t				last_max;

	fprintf(stderr, "Ants: %zu\n", anthil->ants);
	last_max = SIZE_MAX;
	while (find_path(anthil))
	{
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
		i = 0;
		paths = create_path_vec(10);
		while (i < anthil->start->out.links->len)
		{
			if (!anthil->start->out.links->elems[i].usable)
			{
				current = create_room_vec(10);
				collect_path(anthil, &current, anthil->start->out.links->elems[i].ptr);
				print_path(current);
				if ((path = add_path(&paths)))
					*path = (struct s_path) {
						.ants = 0,
						.path = current
					};
			}
			i++;
		}
		if ((i = distribute_ants(anthil, paths)) < last_max)
		{
			last_max = i;
			anthil->paths = paths;
		}
		/*else
		{
			//break ;
		}*/
		unmark(anthil->start);
	}
}
