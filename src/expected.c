/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expected.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 04:36:49 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 04:46:53 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include <unistd.h>

static ssize_t	expected_turns(struct s_anthil *anthil)
{
	if (anthil->end_comments
			&& strncmp(anthil->end_comments, GENERATOR_COMMENT,
				sizeof(GENERATOR_COMMENT) - 1) == 0)
	{
		return (atoi(anthil->end_comments + sizeof(GENERATOR_COMMENT) - 1));
	}
	else
		return (-1);
}

int				check_expected(struct s_anthil *anthil, ssize_t turns)
{
	ssize_t	expected;

	expected = expected_turns(anthil);
	fprintf(stderr, "Turns: %zu, Expected: %zd\n", turns, expected);
	if (expected != -1 && turns > expected)
	{
		write(STDERR_FILENO, MORE_ANTS, sizeof(MORE_ANTS) - 1);
		return (1);
	}
	else
		return (0);
}
