/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 09:33:33 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/20 09:34:51 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STR_H
# define STR_H

# include <stddef.h>
# include <stdbool.h>

typedef struct	s_str {
	size_t	len;
	size_t	capacity;
	char	*inner;
}				t_str;

bool 			str_init(t_str *str, size_t capacity);
bool			str_append(t_str *str, char c);

#endif
