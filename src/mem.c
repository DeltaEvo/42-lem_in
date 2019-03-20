/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 09:14:49 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/20 09:15:52 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mem.h"
#include <stdlib.h>

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const char	*c_src = src;
	char		*c_dst;

	c_dst = dst;
	while (n--)
		*c_dst++ = *c_src++;
	return (dst);
}

void	*ft_realloc(void *o_ptr, size_t oldsize, size_t newsize)
{
	void	*ptr;

	if (!(ptr = malloc(newsize)))
	{
		free(o_ptr);
		return (NULL);
	}
	if (oldsize)
	{
		ft_memcpy(ptr, o_ptr, oldsize);
		free(o_ptr);
	}
	return (ptr);
}
