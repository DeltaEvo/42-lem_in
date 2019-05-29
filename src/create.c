/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 03:38:03 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 03:39:34 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "hashtable.h"
#include <stdlib.h>

struct s_link_vec		*create_link_vec(size_t capacity)
{
	struct s_link_vec	*vec;

	if (!(vec = malloc(sizeof(*vec) + capacity * sizeof(*vec->elems))))
		return (NULL);
	*vec = (struct s_link_vec) {
		.capacity = capacity,
		.len = 0
	};
	return (vec);
}

struct s_path_vec		*create_path_vec(size_t capacity)
{
	struct s_path_vec	*vec;

	if (!(vec = malloc(sizeof(*vec) + capacity * sizeof(*vec->paths))))
		return (NULL);
	*vec = (struct s_path_vec) {
		.capacity = capacity,
		.len = 0
	};
	return (vec);
}

struct s_room_vec		*create_room_vec(size_t capacity)
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

struct s_hashtable		*create_hashtable(size_t size)
{
	struct s_hashtable	*hash;
	size_t				i;

	if (!(hash = malloc(sizeof(*hash) + size * sizeof(*hash->bucket))))
		return (NULL);
	i = 0;
	while (i < size)
	{
		hash->bucket[i].key = NULL;
		i++;
	}
	hash->size = size;
	return (hash);
}
