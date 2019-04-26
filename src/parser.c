/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 08:57:12 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/04/26 16:03:54 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "reader.h"
#include "str.h"
#include "hashtable.h"
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
	return (c == ' ' || c == '\t');
}

static void	skip_ws(t_reader *r)
{
	while (is_ws(io_peek(r)))
		r->index++;
}

static void	skip_nl(t_reader *r)
{
	while (io_peek(r) == '\n')
		r->index++;
}

char	*read_name(t_reader *r)
{
	int16_t	c;
	t_str	str;

	if (!str_init(&str, 8))
		return (NULL);
	while ((c = io_peek(r)) != -1 && !is_ws(c) && c != '-' && c != '\n')
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
	io_readnum(r, &room->x);
	skip_ws(r);
	io_readnum(r, &room->y);
}

struct s_link {
	char	*first;
	char	*second;
};

bool	read_object(t_reader *r, struct s_room **room, struct s_link *link)
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
		link->first = name;
		link->second = read_name(r);
	}
	else
	{
		free(name);
		return (false);
	}
	return (true);
}

bool	link_anthil(struct s_hashtable *hashtable, struct s_link *link)
{
	struct s_entry		*first;
	struct s_entry		*second;
	struct s_room		**room;

	if (!(first = hashtable_get(hashtable, link->first)))
		warning("Room \"%s\" (first part) not found\n", link->first);
	if (!(second = hashtable_get(hashtable, link->second)))
		warning("Room \"%s\" (second part) not found\n", link->second);
	if (!first || !second)
		return (true);
	if ((room = add_room(&((struct s_room *)first->value)->links)))
		*room = second->value;
	if ((room = add_room(&((struct s_room *)second->value)->links)))
		*room = first->value;
	return (true);
}

void	free_room(struct s_room *room)
{
	free(room->name);
	free(room->links);
	free(room);
}

void	free_unused(struct s_anthil *anthil, struct s_hashtable *hashtable)
{
	size_t			i;
	struct s_room	*room;

	i = 0; 
	while (i < hashtable->size)
	{
		if (hashtable->bucket[i].key)
		{
			room = (struct s_room *)hashtable->bucket[i].value;
			if (room != anthil->start && room->links->len == 0)
			{
				warning("Room \"%s\" has 0 links\n", room->name);
				free_room(room);
			}
		}
		i++;
	}
}

struct s_anthil	read_anthil(t_reader *r)
{
	struct s_anthil		anthil;
	enum e_comment		comment;
	struct s_room		*room;
	struct s_link		link;
	struct s_hashtable	*hashtable;
	struct s_entry		*entry;
	bool				start;
	bool				end;
	int32_t				ants;

	anthil.start = NULL;
	while (read_comment(r) != COMMENT_NONE)
		;
	if (!io_readnum(r, &ants) || ants <= 0 || !io_expect(r, "\n"))
	{
		error("Invalid ant num\n");
		return (anthil);
	}
	anthil.ants = ants;
	hashtable = create_hashtable(1);
	while (true)
	{
		skip_nl(r);
		start = false;
		end = false;
		while ((comment = read_comment(r)) != COMMENT_NONE)
			if (comment == COMMENT_START)
				start = true;
			else if (comment == COMMENT_END)
				end = true;
		room = NULL;
		if (!read_object(r, &room, &link))
			break ;
		if (room)
		{
			if (start)
			{
				if (anthil.start)
					warning("Start redefined taking new value \"%s\" old was \"%s\"\n", room->name, anthil.start->name);
				anthil.start = room;
			}
			if (end)
				room->end = true;
			room->links = create_vec(1);
			if ((entry = hashtable_insert(&hashtable, create_entry(room->name))))
				entry->value = room;
			else if (hashtable_get(hashtable, room->name))
			{
				error("Duplicate room \"%s\"\n", room->name);
				return (anthil);
			}
		}
		else if (!link_anthil(hashtable, &link))
			return (anthil);
		if (!io_expect(r, "\n"))
		{
			error("Too much data\n");
			return (anthil);
		}
	}
	free_unused(&anthil, hashtable);
	return (anthil);
}
