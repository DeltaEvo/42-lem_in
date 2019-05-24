/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 09:28:56 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/24 16:50:37 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "str.h"
#include "mem.h"
#include <stdlib.h>

bool	str_init(t_str *str, size_t capacity)
{
	char	*inner;

	if (!(inner = malloc(capacity)))
		return (false);
	*str = (t_str) {
		.capacity = capacity,
		.len = 0,
		.inner = inner
	};
	return (true);
}

bool	str_append(t_str *str, char c)
{
	size_t	new_capacity;

	if (str->len == str->capacity)
	{
		new_capacity = str->capacity * 2;
		str->inner = ft_realloc(str->inner, str->capacity, new_capacity);
		if (!str->inner)
			return (false);
		str->capacity = new_capacity;
	}
	str->inner[str->len++] = c;
	return (true);
}
