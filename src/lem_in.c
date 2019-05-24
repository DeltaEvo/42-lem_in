/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 08:28:35 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/24 17:21:59 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "reader.h"
#include "lem_in.h"

size_t	print_moves(struct s_anthil *anthil)
{
	bool	moving;
	size_t	turn;
	size_t	i;
	size_t	j;
	size_t	ants;
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
			ants = anthil->paths->paths[i].ants;
			/*if (turn == 1)
			{
				printf("%zu -> %zu (%zu)\n", i, ants, anthil->ants);
			}*/
			j = 0;
			while (j < ants && j < turn)
			{
				if (turn - j >= anthil->paths->paths[i].path->len)
				{
					j++;
					continue ;
				}
				if (moving)
					printf(" ");
				printf("L%zu-%s", ants_offset + j + 1, anthil->paths->paths[i].path->rooms[turn - j].ptr->name);
				moving = true;
				j++;
			}
			ants_offset += ants;
			i++;
		}
		if (moving)
		{
			printf("\n");
			fflush(stdout);
		}
		turn++;
	}
	return (turn - 2);
}

#define GENERATOR_COMMENT "Here is the number of lines required: "

ssize_t	expected_turns(struct s_anthil *anthil)
{
	if (anthil->end_comments
		&& strncmp(anthil->end_comments, GENERATOR_COMMENT, sizeof(GENERATOR_COMMENT) - 1) == 0)
	{
		return (atoi(anthil->end_comments + sizeof(GENERATOR_COMMENT) - 1));
	}
	else
		return (-1);
}

int	main(void)
{
	t_reader		r;
	struct s_anthil	anthil;

	r = io_create_reader(0);
	anthil = (struct s_anthil) {
		.paths = NULL,
	};
	if (!read_anthil(&r, &anthil))
		return (1);
	if (!anthil.start)
	{
		error("No start link\n");
		return (1);
	}
	if (anthil.start == anthil.end)
	{
		error("Start is end\n");
		return (1);
	}
	find_all_paths(&anthil);
	if (!anthil.paths)
	{
		error("No paths found\n");
		return (1);
	}
	print_anthil(anthil);
	size_t turns = print_moves(&anthil);
	fprintf(stderr, "Turns: %zu, Expected: %zd\n", turns, expected_turns(&anthil));
	if (turns > expected_turns(&anthil))
	{
		fprintf(stderr, "\33[31mMore than expected\n\33[0m");
		return (1);
	}
}

