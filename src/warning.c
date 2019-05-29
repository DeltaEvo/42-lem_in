/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   warning.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 05:26:23 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 05:58:04 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "parser.h"
#include "mem.h"
#include <stdlib.h>

void		warning_not_found(bool first, char *name)
{
	write(STDERR_FILENO, WARN_NOT_FOUND_PRE, sizeof(WARN_NOT_FOUND_PRE) - 1);
	write(STDERR_FILENO, name, ft_strlen(name));
	write(STDERR_FILENO, first ? "\" (first part)" : "\" (second part)",
			first ? 14 : 15);
	write(STDERR_FILENO, WARN_NOT_FOUND_POST, sizeof(WARN_NOT_FOUND_POST) - 1);
}

void		warning_redefined(bool start, char *new, char *old)
{
	write(STDERR_FILENO, WARNING, sizeof(WARNING) - 1);
	write(STDERR_FILENO, start ? "Start" : "End", start ? 5 : 3);
	write(STDERR_FILENO, WARN_REDEFINED_1, sizeof(WARN_REDEFINED_1) - 1);
	write(STDERR_FILENO, new, ft_strlen(new));
	write(STDERR_FILENO, WARN_REDEFINED_2, sizeof(WARN_REDEFINED_2) - 1);
	write(STDERR_FILENO, old, ft_strlen(old));
	write(STDERR_FILENO, WARN_REDEFINED_3, sizeof(WARN_REDEFINED_3) - 1);
}

static void	warning_0_links(char *name)
{
	write(STDERR_FILENO, WARN_0_LINKS_PRE, sizeof(WARN_0_LINKS_PRE) - 1);
	write(STDERR_FILENO, name, ft_strlen(name));
	write(STDERR_FILENO, WARN_0_LINKS_POST, sizeof(WARN_0_LINKS_POST) - 1);
}

bool		free_and_warn_unused(struct s_hashtable *hashtable)
{
	size_t			i;
	struct s_room	*room;

	i = 0;
	while (i < hashtable->size)
	{
		if (hashtable->bucket[i].key)
		{
			room = (struct s_room *)hashtable->bucket[i].value;
			if (room->out.links->len == 0)
				warning_0_links(room->name);
		}
		i++;
	}
	free(hashtable);
	return (true);
}
