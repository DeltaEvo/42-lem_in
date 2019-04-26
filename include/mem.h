/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 09:16:01 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/04/26 14:01:44 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEM_H
# define MEM_H

# include <stddef.h>

void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_realloc(void *o_ptr, size_t oldsize, size_t newsize);
int		ft_strcmp(const char *s1, const char *s2);

#endif
