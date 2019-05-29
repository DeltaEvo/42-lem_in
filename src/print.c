/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 10:23:07 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 04:43:01 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "mem.h"
#include <unistd.h>

void	print_comments(const char *comments)
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

void	print_room(struct s_room *room)
{
	size_t	i;

	room->mark = true;
	print_comments(room->comments);
	write(STDOUT_FILENO, room->name, ft_strlen(room->name));
	write(STDOUT_FILENO, " ", 1);
	putnbr_fd(STDOUT_FILENO, room->x);
	write(STDOUT_FILENO, " ", 1);
	putnbr_fd(STDOUT_FILENO, room->y);
	i = 0;
	while (i < room->out.links->len)
	{
		if (!get_room(room->out.links->elems[i].ptr)->mark)
			print_room(get_room(room->out.links->elems[i].ptr));
		i++;
	}
}

void	print_links(struct s_room *room)
{
	size_t	i;

	room->mark = true;
	i = 0;
	while (i < room->out.links->len)
	{
		if (!room->out.links->elems[i].virtual && !get_room(room->out.links->elems[i].ptr)->mark)
		{
			print_comments(room->out.links->elems[i].comments);
			if (room->out.links->elems[i].first)
				write(STDOUT_FILENO, room->name, ft_strlen(room->name));
			else
				write(STDOUT_FILENO, get_room(room->out.links->elems[i].ptr)->name, ft_strlen(get_room(room->out.links->elems[i].ptr)->name));
			write(STDOUT_FILENO, "-", 1);
			if (room->out.links->elems[i].first)
				write(STDOUT_FILENO, get_room(room->out.links->elems[i].ptr)->name, ft_strlen(get_room(room->out.links->elems[i].ptr)->name));
			else
				write(STDOUT_FILENO, room->name, ft_strlen(room->name));
			write(STDOUT_FILENO, "\n", 1);
		}
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

void	print_anthil(struct s_anthil *anthil)
{
	print_comments(anthil->start_comments);
	putnbr_fd(STDOUT_FILENO, anthil->ants);
	print_room(anthil->start);
	unmark(anthil->start);
	print_links(anthil->start);
	unmark(anthil->start);
	print_comments(anthil->end_comments);
	write(STDOUT_FILENO, "\n", 1);
}
