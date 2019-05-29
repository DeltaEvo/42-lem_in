/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 09:28:56 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 03:41:31 by dde-jesu         ###   ########.fr       */
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
	char	*new;

	if (str->len == str->capacity)
	{
		new_capacity = str->capacity * 2;
		new = ft_realloc(str->inner, str->capacity, new_capacity);
		if (!new)
			return (false);
		str->inner = new;
		str->capacity = new_capacity;
	}
	str->inner[str->len++] = c;
	return (true);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}
