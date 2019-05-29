/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 08:28:35 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 05:59:48 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "reader.h"
#include "lem_in.h"
#include "mem.h"

static bool		is_special(struct s_anthil *anthil)
{
	size_t	i;

	i = 0;
	while (i < anthil->start->out.links->len)
	{
		if (anthil->start->out.links->elems[i].ptr == &anthil->end->in)
			return (true);
		i++;
	}
	return (false);
}

static ssize_t	handle_special(struct s_anthil *anthil)
{
	int32_t	i;

	i = 0;
	while (i < anthil->ants)
	{
		print_move(i + 1, anthil->end->name);
		i++;
		if (i < anthil->ants)
			write(STDOUT_FILENO, " ", 1);
	}
	write(STDOUT_FILENO, "\n", 1);
	return (1);
}

static ssize_t	handle_normal(struct s_anthil *anthil)
{
	find_all_paths(anthil);
	if (!anthil->paths)
	{
		write(STDERR_FILENO, ERROR_NO_PATH, sizeof(ERROR_NO_PATH) - 1);
		return (-1);
	}
	print_anthil(anthil);
	return (print_moves(anthil));
}

static int		run(struct s_anthil *anthil, bool check)
{
	ssize_t	turns;

	if (!anthil->start)
		write(STDERR_FILENO, ERROR_NO_START, sizeof(ERROR_NO_START) - 1);
	else if (!anthil->end)
		write(STDERR_FILENO, ERROR_NO_END, sizeof(ERROR_NO_END) - 1);
	else if (anthil->start == anthil->end)
		write(STDERR_FILENO, ERROR_START_END, sizeof(ERROR_START_END) - 1);
	else
	{
		if (is_special(anthil))
			turns = handle_special(anthil);
		else
			turns = handle_normal(anthil);
		if (turns == -1)
			return (1);
		else
			return (check ? check_expected(anthil, turns) : 0);
	}
	return (1);
}

int				main(int argc, char *argv[])
{
	t_reader		r;
	struct s_anthil	anthil;
	int				ret;

	r = io_create_reader(0);
	anthil = (struct s_anthil) {
		.rooms = create_room_vec(16)
	};
	if (!anthil.rooms)
		return (1);
	if (read_anthil(&r, &anthil))
		ret = run(&anthil, argc == 2 && ft_strcmp(argv[1], "--check") == 0);
	else
		ret = 1;
	free_anthil(&anthil);
	return (ret);
}
