/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 10:23:07 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 05:51:22 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "mem.h"

static void	print_comments(const char *comments)
{
	const char	*end;

	if (!comments)
		return ;
	while (*comments)
	{
		end = ft_strchr(comments, '\n');
		if (!end)
			end = comments + ft_strlen(comments);
		write(STDOUT_FILENO, "#", 1);
		write(STDOUT_FILENO, comments, end - comments);
		write(STDOUT_FILENO, "\n", 1);
		comments = end + 1;
	}
}

static void	print_link(struct s_link *link, struct s_room *room)
{
	print_comments(link->comments);
	if (link->first)
		write(STDOUT_FILENO, room->name, ft_strlen(room->name));
	else
		write(STDOUT_FILENO, get_room(link->ptr)->name,
				ft_strlen(get_room(link->ptr)->name));
	write(STDOUT_FILENO, "-", 1);
	if (link->first)
		write(STDOUT_FILENO, get_room(link->ptr)->name,
				ft_strlen(get_room(link->ptr)->name));
	else
		write(STDOUT_FILENO, room->name, ft_strlen(room->name));
	write(STDOUT_FILENO, "\n", 1);
}

static void	print_links(struct s_room *room)
{
	size_t	i;

	room->mark = true;
	i = 0;
	while (i < room->out.links->len)
	{
		if (!room->out.links->elems[i].virtual
				&& !get_room(room->out.links->elems[i].ptr)->mark)
			print_link(room->out.links->elems + i, room);
		i++;
	}
	i = 0;
	while (i < room->out.links->len)
	{
		if (!get_room(room->out.links->elems[i].ptr)->mark)
			print_links(get_room(room->out.links->elems[i].ptr));
		i++;
	}
}

void		print_anthil(struct s_anthil *anthil)
{
	size_t	i;

	print_comments(anthil->start_comments);
	putnbr_fd(STDOUT_FILENO, anthil->ants);
	write(STDOUT_FILENO, "\n", 1);
	i = 0;
	while (i < anthil->rooms->len)
	{
		print_comments(anthil->rooms->rooms[i]->comments);
		write(STDOUT_FILENO, anthil->rooms->rooms[i]->name,
				ft_strlen(anthil->rooms->rooms[i]->name));
		write(STDOUT_FILENO, " ", 1);
		putnbr_fd(STDOUT_FILENO, anthil->rooms->rooms[i]->x);
		write(STDOUT_FILENO, " ", 1);
		putnbr_fd(STDOUT_FILENO, anthil->rooms->rooms[i]->y);
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	unmark(anthil->start);
	print_links(anthil->start);
	unmark(anthil->start);
	print_comments(anthil->end_comments);
	write(STDOUT_FILENO, "\n", 1);
}
