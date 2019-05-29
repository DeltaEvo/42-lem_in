/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 09:03:08 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 04:44:57 by dde-jesu         ###   ########.fr       */
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

# define ERROR_MALLOC ERROR "Error malloc\n"
# define ERROR_NO_START ERROR "No start link\n"
# define ERROR_NO_END ERROR "No end link\n"
# define ERROR_START_END ERROR "Start is end\n"
# define ERROR_NO_PATH ERROR "No paths found\n"

# define GENERATOR_COMMENT "Here is the number of lines required: "
# define MORE_ANTS CSI_RED "More than expected\n" CSI_RESET

# include <stdio.h>
# define warning(...) fprintf(stderr, WARNING __VA_ARGS__)
# define error(...) fprintf(stderr, ERROR __VA_ARGS__)

struct				s_link {
	struct s_node	*ptr;
	bool			first;
	bool			virtual;
	bool			usable;
	char			*comments;
};

struct				s_link_vec {
	size_t				len;
	size_t				capacity;
	struct s_link		elems[];
};

struct				s_node {
	bool				in;
	struct s_node		*prev;
	struct s_link_vec	*links;
};

struct				s_room {
	char				*name;
	char				*comments;
	int32_t				x;
	int32_t				y;
	bool				mark;
	struct s_node		in;
	struct s_node		out;
};

struct				s_room_vec {
	size_t			len;
	size_t			capacity;
	struct s_room	*rooms[];
};

struct				s_path {
	int32_t				ants;
	struct s_room_vec	*path;
};

struct				s_path_vec {
	size_t			len;
	size_t			capacity;
	struct s_path	paths[];
};

struct				s_anthil {
	char				*start_comments;
	char				*end_comments;
	int32_t				ants;
	struct s_room		*start;
	struct s_room		*end;
	struct s_path_vec	*paths;
	struct s_room_vec	*rooms;
};


struct				s_link_names {
	char			*first;
	char			*second;
};

void    			find_all_paths(struct s_anthil *anthil);
void				print_anthil(struct s_anthil *anthil);
void				unmark(struct s_room *room);
struct s_link		*add_link(struct s_link_vec **vec);
struct s_path		*add_path(struct s_path_vec **vec);
struct s_room		**add_room(struct s_room_vec **vec);
struct s_link_vec	*create_link_vec(size_t capacity);
struct s_path_vec	*create_path_vec(size_t capacity);
struct s_room_vec	*create_room_vec(size_t capacity);
struct s_room		*get_room(struct s_node *node);
bool				free_room(struct s_room *room);
void				free_anthil(struct s_anthil *anthil);
void				*free_paths(struct s_path_vec *paths);
bool				ffree(void *ptr);
void				*error_malloc(void *ptr);
size_t				print_moves(struct s_anthil *anthil);
void				print_move(size_t ant, char *name);
int					check_expected(struct s_anthil *anthil, ssize_t turns);
void				putnbr_fd(int fd, intmax_t n);

#endif
