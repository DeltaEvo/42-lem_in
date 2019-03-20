/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 08:57:12 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/20 11:46:27 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "reader.h"
#include "str.h"
#include <stdbool.h>
#include <stdlib.h>

enum	e_comment
{
	COMMENT_NONE,
	COMMENT_REAL,
	COMMENT_START,
	COMMENT_END
};

static bool	is_ws(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static void	skip_ws(t_reader *r)
{
	while (is_ws(io_peek(r)))
		r->index++;
}

char	*read_name(t_reader *r)
{
	int16_t	c;
	t_str	str;

	if (!str_init(&str, 8))
		return (NULL);
	while ((c = io_peek(r)) != -1 && !is_ws(c) && c != '-')
	{
		if (!str_append(&str, c))
			return (NULL);
		r->index++;
	}
	str_append(&str, '\0');
	return (str.inner);
}

enum e_comment	read_comment(t_reader *r)
{
	int16_t	c;
	if (io_peek(r) == '#')
	{
		r->index++;
		if (io_peek(r) == '#')
		{
			r->index++;
			c = io_peek(r);
			if (c == 's' && io_expect(r, "start\n"))
				return (COMMENT_START);
			if (c == 'e' && io_expect(r, "end\n"))
				return (COMMENT_END);
		}
		io_skip_until(r, '\n');
		return (COMMENT_REAL);
	}
	else
		return (COMMENT_NONE);
}

void	 read_room_coords(t_reader *r, struct s_room *room)
{
	skip_ws(r);
	room->x = io_readnum(r);
	skip_ws(r);
	room->y = io_readnum(r);
}

bool	read_object(t_reader *r, struct s_room **room)
{
	char			*name;
	int16_t			c;

	name = read_name(r);
	c = io_peek(r);
	if (is_ws(c))
	{
		if (!(*room = malloc(sizeof(**room))))
		{
			free(name);
			return (false);
		}
		(*room)->name = name;
		read_room_coords(r, *room);
	}
	else if (c == '-')
	{
		r->index++;
		printf("Link %s - %s\n", name, read_name(r));
	}
	else
	{
		free(name);
		return (false);
	}
	return (true);
}

struct s_anthil	read_anthil(t_reader *r)
{
	struct s_anthil	anthil;
	enum e_comment	comment;
	struct s_room	*room;

	anthil = (struct s_anthil) {
		.start = NULL,
		.end = NULL
	};
	while (true)
	{
		skip_ws(r);
		comment = read_comment(r);
		room = NULL;
		if (!read_object(r, &room))
			break ;
		if (room)
		{
			if (comment == COMMENT_START)
				printf("Start ");
			if (comment == COMMENT_END)
				printf("End ");
			printf("Room %s %d %d\n", room->name, room->x, room->y);
		}
	}
	return (anthil);
}
