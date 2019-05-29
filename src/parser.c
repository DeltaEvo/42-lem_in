/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 08:57:12 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 05:59:26 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "str.h"
#include "mem.h"
#include <stdbool.h>
#include <stdlib.h>

static bool		init_and_read_room(t_reader *r, struct s_room *room, char *name)
{
	struct s_link		*link;

	*room = (struct s_room) {
		.name = name,
		.in = {
			.in = true, .links = create_link_vec(1)
		},
		.out = {
			.in = false, .links = create_link_vec(1)
		}
	};
	if (!room->in.links || !room->out.links
			|| !(link = add_link(&room->in.links)))
		return (false);
	link->comments = NULL;
	link->first = false;
	link->virtual = true;
	link->usable = true;
	link->ptr = &room->out;
	skip_ws(r);
	io_readnum(r, &room->x);
	skip_ws(r);
	io_readnum(r, &room->y);
	return (true);
}

static bool		read_object(t_reader *r, struct s_room **room,
		struct s_link_names *link)
{
	char			*name;

	if (!(name = read_name(r)))
		return (false);
	if (io_peek(r) == ' ' || io_peek(r) == '\t')
	{
		if (*name == 'L')
		{
			error_illegal(name);
			return (ffree(name));
		}
		if (!(*room = malloc(sizeof(**room)))
				|| !init_and_read_room(r, *room, name))
			return ((bool)error_malloc(name));
	}
	else if (io_peek(r) == '-')
	{
		r->index++;
		link->first = name;
		if (!(link->second = read_name(r)))
			return (ffree(name));
	}
	else
		return (ffree(name));
	return (true);
}

static bool		link_anthil(struct s_hashtable *hashtable, struct s_link_names *links,
		char **o_comments, struct s_link *link)
{
	const char			*comments = *o_comments;
	struct s_entry		*first;
	struct s_entry		*second;

	*o_comments = NULL;
	if (!(first = hashtable_get(hashtable, links->first)))
		warning_not_found(true, links->first);
	if (!(second = hashtable_get(hashtable, links->second)))
		warning_not_found(false, links->second);
	(ffree(links->first) || ffree(links->second));
	if (!first || !second)
		return (true);
	if (!(link = add_link(&((struct s_room *)first->value)->out.links)))
		return ((bool)error_malloc(NULL));
	*link = (struct s_link) {
		.ptr = &((struct s_room *)second->value)->in, .first = true,
		.virtual = false, .usable = true, .comments = (char *)comments
	};
	if (!(link = add_link(&((struct s_room *)second->value)->out.links)))
		return ((bool)error_malloc(NULL));
	*link = (struct s_link) {
		.ptr = &((struct s_room *)first->value)->in, .first = false,
		.virtual = false, .usable = true, .comments = (char *)comments
	};
	return (true);
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
			warning_redefined(true, room->name, anthil->start->name);
		anthil->start = room;
	}
	if (has_command(room->comments, "#end"))
	{
		if (anthil->end)
			warning_redefined(false, room->name, anthil->end->name);
		anthil->end = room;
	}
	if (!(entry = hashtable_insert(hashtable, create_entry(room->name))))
		return (error_duplicate(room->name) && free_room(room));
	entry->value = room;
	if (!(ptr = add_room(&anthil->rooms)))
		return (free_room(room));
	*ptr = room;
	return (true);
}

bool			read_anthil(t_reader *r, struct s_anthil *anthil)
{
	struct s_room		*room;
	struct s_link_names	link;
	struct s_hashtable	*table;

	anthil->start_comments = read_comments(r);
	if (!io_readnum(r, &anthil->ants) || anthil->ants <= 0
			|| !io_expect(r, "\n"))
		return (error_invalid_ants() && false);
	if (!(table = create_hashtable(10)))
		return (false);
	while (!(room = NULL))
	{
		skip_nl(r);
		anthil->end_comments = read_comments(r);
		if (io_peek(r) == -1)
			break ;
		if (!read_object(r, &room, &link) || (room &&
					!handle_room(&table, anthil, room, &anthil->end_comments)))
			return (ffree(table));
		if (!room && !link_anthil(table, &link, &anthil->end_comments, 0))
			return (error_malloc(NULL) || ffree(table));
		if (!io_expect(r, "\n"))
			return (error_too_much_data() && ffree(table));
	}
	return (free_and_warn_unused(table));
}
