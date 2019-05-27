/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 08:57:12 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/27 11:30:44 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "str.h"
#include "mem.h"
#include "hashtable.h"
#include <stdbool.h>
#include <stdlib.h>

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

char	*read_comments(t_reader *r)
{
	int16_t	c;
	t_str	str;

	if (io_peek(r) == '#')
	{
		if (!str_init(&str, 8))
			return (NULL);
		r->index++;
		while ((c = io_peek(r)) != -1)
		{
			if (!str_append(&str, c))
				return ((void *)ffree(str.inner));
			r->index++;
			if (c == '\n')
			{
				if (io_peek(r) == '#')
					r->index++;
				else
					break ;
			}
		}
		str_append(&str, '\0');
		return (str.inner);
	}
	return (NULL);
}

void	init_and_read_room(t_reader *r, struct s_room *room, char *name)
{
	struct s_link		*link;

	*room = (struct s_room) {
		.name = name,
		.in = {
			.in = true,
			.links = create_link_vec(1),
		},
		.out = {
			.in = false,
			.links = create_link_vec(1),
		}
	};
	if ((link = add_link(&room->in.links)))
	{
		link->comments = NULL;
		link->first = false;
		link->virtual = true;
		link->usable = true;
		link->ptr = &room->out;
	}
	skip_ws(r);
	io_readnum(r, &room->x);
	skip_ws(r);
	io_readnum(r, &room->y);
}

bool	read_object(t_reader *r, struct s_room **room, struct s_link_names *link)
{
	char			*name;
	int16_t			c;

	name = read_name(r);
	if (is_ws(c = io_peek(r)))
	{
		if (!(*room = malloc(sizeof(**room))))
		{
			free(name);
			return (false);
		}
		init_and_read_room(r, *room, name);
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

bool	link_anthil(struct s_hashtable *hashtable, struct s_link_names *links,
		char **o_comments)
{
	const char			*comments = *o_comments;
	struct s_entry		*first;
	struct s_entry		*second;
	struct s_link		*link;

	*o_comments = NULL;
	if (!(first = hashtable_get(hashtable, links->first)))
		warning("Room \"%s\" (first part) not found\n", links->first);
	if (!(second = hashtable_get(hashtable, links->second)))
		warning("Room \"%s\" (second part) not found\n", links->second);
	free(links->first);
	free(links->second);
	if (!first || !second)
		return (true);
	if (!(link = add_link(&((struct s_room *)first->value)->out.links)))
		return (false);
	link->ptr = &((struct s_room *)second->value)->in;
	link->first = true;
	link->virtual = false;
	link->usable = true;
	link->comments = comments;
	if (!(link = add_link(&((struct s_room *)second->value)->out.links)))
		return (false);
	link->ptr = &((struct s_room *)first->value)->in;
	link->first = false;
	link->virtual = false;
	link->usable = true;
	link->comments = comments;
	return (true);
}

bool	free_and_warn_unused(struct s_anthil *anthil, struct s_hashtable *hashtable)
{
	size_t			i;
	struct s_room	*room;

	i = 0;
	while (i < hashtable->size)
	{
		if (hashtable->bucket[i].key)
		{
			room = (struct s_room *)hashtable->bucket[i].value;
			if (room != anthil->start && room->out.links->len == 0)
				warning("Room \"%s\" has 0 links\n", room->name);
		}
		i++;
	}
	free(hashtable);
	return (true);
}

bool	has_command(const char *comments, const char *command)
{
	const size_t	command_len = ft_strlen(command);
	const char		*end;

	if (!comments)
		return (false);
	while (*comments)
	{
		end = ft_strchr(comments, '\n');
		if (!end)
			end = comments + ft_strlen(comments);
		if ((size_t)(end - comments) == command_len
			&& ft_memcmp(comments, command, command_len) == 0)
			return (true);
		comments = end + 1;
	}
	return (false);
}

static bool		handle_room(struct s_hashtable **hashtable,
		struct s_anthil *anthil, struct s_room *room, char **comments)
{
	struct s_entry		*entry;
	struct s_room		**ptr;

	room->comments = *comments;
	*comments = NULL;
	if (has_command(room->comments, "#start"))
	{
		if (anthil->start)
			warning("Start redefined taking new value \"%s\" old was \"%s\"\n",
					room->name, anthil->start->name);
		anthil->start = room;
	}
	if (has_command(room->comments, "#end"))
	{
		if (anthil->end)
			warning("End redefined taking new value \"%s\" old was \"%s\"\n",
					room->name, anthil->end->name);
		anthil->end = room;
	}
	if (!(entry = hashtable_insert(hashtable, create_entry(room->name))))
		return (error("Duplicate room \"%s\"\n", room->name) && free_room(room));
	entry->value = room;
	if (!(ptr = add_room(&anthil->rooms)))
		return (free_room(room));
	*ptr = room;
	return (true);
}

bool	read_anthil(t_reader *r, struct s_anthil *anthil)
{
	struct s_room		*room;
	struct s_link_names	link;
	struct s_hashtable	*table;

	anthil->start_comments = read_comments(r);
	if (!io_readnum(r, &anthil->ants) || anthil->ants <= 0
			|| !io_expect(r, "\n"))
		return (error("Invalid ant num\n") && false);
	if (!(table = create_hashtable(10)))
		return (false);
	while (true)
	{
		skip_nl(r);
		anthil->end_comments = read_comments(r);
		room = NULL;
		if (!read_object(r, &room, &link))
			break ;
		if (room && !handle_room(&table, anthil, room, &anthil->end_comments))
			return (ffree(table));
		if (!room && !link_anthil(table, &link, &anthil->end_comments))
			return (error("Malloc\n") && ffree(table));
		if (!io_expect(r, "\n"))
			return (error("Too much data\n") && ffree(table));
	}
	return (free_and_warn_unused(anthil, table));
}
