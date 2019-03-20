/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   links.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 09:08:09 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/20 10:14:46 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "mem.h"
#include <stdlib.h>

struct s_room_vec 	*create_vec(size_t capacity)
{
	struct s_room_vec	*vec;

	if (!(vec = malloc(sizeof(*vec) + capacity * sizeof(*vec->rooms))))
		return (NULL);
	*vec = (struct s_room_vec) {
		.capacity = capacity,
		.len = 0
	};
	return (vec);
}

struct s_room		**add_room(struct s_room_vec **vec)
{
	size_t	new_capacity;

	if ((*vec)->len == (*vec)->capacity)
	{
		new_capacity = (*vec)->capacity * 2;
		*vec = ft_realloc(*vec,
				sizeof(**vec) + (*vec)->capacity * sizeof(*(*vec)->rooms),
				sizeof(**vec) + new_capacity * sizeof(*(*vec)->rooms));
		if (!*vec)
			return (NULL);
		(*vec)->capacity = new_capacity;
	}
	return (*vec)->rooms + (*vec)->len++;
}

