/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 09:14:49 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 03:57:51 by dde-jesu         ###   ########.fr       */
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
		return (NULL);
	if (oldsize)
	{
		ft_memcpy(ptr, o_ptr, oldsize);
		free(o_ptr);
	}
	return (ptr);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	size_t i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *c_s1 = s1;
	const unsigned char *c_s2 = s2;

	if (n != 0)
		while (n--)
			if (*c_s1++ != *c_s2++)
				return (*--c_s1 - *--c_s2);
	return (0);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
		if (*s++ == c)
			return ((char *)s - 1);
	if (c)
		return (0);
	else
		return ((char *)s);
}
