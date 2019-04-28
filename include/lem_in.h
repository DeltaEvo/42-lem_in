/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 09:03:08 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/04/28 16:09:14 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include <stddef.h>
# include <stdint.h>
# include <stdbool.h>

# define CSI "\033["
# define CSI_GREEN CSI "32;01m"
# define CSI_WHITE CSI "37;01m"
# define CSI_BLUE CSI "34;01m"
# define CSI_YELLOW CSI "33;01m"
# define CSI_RED CSI "31m"
# define CSI_RESET CSI "0m"

# define WARNING CSI_YELLOW "Warning: " CSI_RESET
# define ERROR CSI_RED "Error: " CSI_RESET

# include <stdio.h>
# define warning(...) fprintf(stderr, WARNING __VA_ARGS__)
# define error(...) fprintf(stderr, ERROR __VA_ARGS__)

struct	s_room {
	char				*name;
	char				*comments;
	int32_t				x;
	int32_t				y;
	bool				end;
	struct s_room_vec	*links;
	bool				mark;
	bool				broken;
	struct s_room		*prev;
	struct s_room		*old_prev;
};

struct	s_room_ptr {
	struct s_room	*ptr;
	char			*comments;
};

struct	s_room_vec {
	size_t				len;
	size_t				capacity;
	struct s_room_ptr	rooms[];
};

struct	s_anthil {
	char				*start_comments;
	char				*end_comments;
	size_t				ants;
	struct s_room		*start;
};

struct s_room_ptr	*add_room(struct s_room_vec **vec);
struct s_room_vec 	*create_vec(size_t capacity);
void				print_anthil(struct s_anthil anthil);

#endif
