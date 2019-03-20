/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 09:03:08 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/20 11:43:53 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include <stddef.h>
# include <stdint.h>

struct	s_room {
	char				*name;
	int32_t				x;
	int32_t				y;
	struct s_room_vec	*links;
};

struct	s_room_vec {
	size_t			len;
	size_t			capacity;
	struct s_room	*rooms[];
};

struct	s_anthil {
	struct s_room	*start;
	struct s_room	*end;
};

#endif
