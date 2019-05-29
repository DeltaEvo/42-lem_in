/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   links.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 09:08:09 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 03:40:48 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "mem.h"

struct s_room	**add_room(struct s_room_vec **vec)
{
	struct s_room_vec	*new;
	size_t				new_capacity;

	if ((*vec)->len == (*vec)->capacity)
	{
		new_capacity = (*vec)->capacity * 2;
		new = ft_realloc(*vec,
				sizeof(**vec) + (*vec)->capacity * sizeof(*(*vec)->rooms),
				sizeof(**vec) + new_capacity * sizeof(*(*vec)->rooms));
		if (!new)
			return (NULL);
		*vec = new;
		(*vec)->capacity = new_capacity;
	}
	return (*vec)->rooms + (*vec)->len++;
}

struct s_link	*add_link(struct s_link_vec **vec)
{
	struct s_link_vec	*new;
	size_t				new_capacity;

	if ((*vec)->len == (*vec)->capacity)
	{
		new_capacity = (*vec)->capacity * 2;
		new = ft_realloc(*vec,
				sizeof(**vec) + (*vec)->capacity * sizeof(*(*vec)->elems),
				sizeof(**vec) + new_capacity * sizeof(*(*vec)->elems));
		if (!new)
			return (NULL);
		*vec = new;
		(*vec)->capacity = new_capacity;
	}
	return (*vec)->elems + (*vec)->len++;
}

struct s_path	*add_path(struct s_path_vec **vec)
{
	struct s_path_vec	*new;
	size_t				new_capacity;

	if ((*vec)->len == (*vec)->capacity)
	{
		new_capacity = (*vec)->capacity * 2;
		new = ft_realloc(*vec,
				sizeof(**vec) + (*vec)->capacity * sizeof(*(*vec)->paths),
				sizeof(**vec) + new_capacity * sizeof(*(*vec)->paths));
		if (!new)
			return (NULL);
		*vec = new;
		(*vec)->capacity = new_capacity;
	}
	return (*vec)->paths + (*vec)->len++;
}
