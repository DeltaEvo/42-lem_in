/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 04:55:59 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 04:59:36 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static struct s_link	*get_link(struct s_node *from, struct s_node *to)
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

void					toggle_usability(struct s_anthil *anthil)
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

size_t					get_shortest_path(struct s_path_vec *paths)
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
