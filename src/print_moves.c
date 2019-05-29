/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_moves.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 04:30:52 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 06:00:05 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "mem.h"

void				putnbr_fd(int fd, intmax_t n)
{
	char		buf[22];
	intmax_t	i;
	const char	neg = n < 0;

	if (!neg)
		n = -n;
	i = sizeof(buf);
	if (!n)
		buf[--i] = '0';
	else
		while (n)
		{
			buf[--i] = -(n % 10) + '0';
			n /= 10;
		}
	if (neg)
		buf[--i] = '-';
	write(fd, buf + i, sizeof(buf) - i);
}

static int32_t		print_moves_path(size_t turn, size_t ants_offset,
		struct s_path *path, bool *moving)
{
	size_t	ants;
	size_t	j;

	ants = path->ants;
	j = 0;
	while (j < ants && j < turn)
	{
		if (turn - j < path->path->len)
		{
			if (*moving)
				write(STDOUT_FILENO, " ", 1);
			print_move(ants_offset + j + 1, path->path->rooms[turn - j]->name);
			*moving = true;
		}
		j++;
	}
	return (ants);
}

void				print_move(size_t ant, char *name)
{
	write(STDOUT_FILENO, "L", 1);
	putnbr_fd(STDOUT_FILENO, ant);
	write(STDOUT_FILENO, "-", 1);
	write(STDOUT_FILENO, name, ft_strlen(name));
}

size_t				print_moves(struct s_anthil *anthil)
{
	bool	moving;
	size_t	turn;
	size_t	i;
	size_t	ants_offset;

	turn = 1;
	moving = true;
	while (moving)
	{
		moving = false;
		i = 0;
		ants_offset = 0;
		while (i < anthil->paths->len)
		{
			ants_offset += print_moves_path(turn, ants_offset,
					anthil->paths->paths + i, &moving);
			i++;
		}
		if (moving)
			write(STDOUT_FILENO, "\n", 1);
		turn++;
	}
	return (turn - 2);
}
