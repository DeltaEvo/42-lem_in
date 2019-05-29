/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expected.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 04:36:49 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 06:00:53 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	if (!n)
		return (0);
	i = 0;
	while (i < n - 1 && s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

static ssize_t	atoi(char *str)
{
	ssize_t	result;
	char	mod;

	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	result = 0;
	mod = *str == '-' || *str == '+' ? *str++ == '-' : 0;
	while (*str >= '0' && *str <= '9')
		result = result * 10 + *str++ - '0';
	return ((int)(mod ? -result : result));
}

static ssize_t	expected_turns(struct s_anthil *anthil)
{
	if (anthil->end_comments
			&& ft_strncmp(anthil->end_comments, GENERATOR_COMMENT,
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
	write(STDERR_FILENO, "Turns: ", 7);
	putnbr_fd(STDERR_FILENO, turns);
	write(STDERR_FILENO, ", Expected: ", 12);
	putnbr_fd(STDERR_FILENO, expected);
	write(STDERR_FILENO, "\n", 1);
	if (expected != -1 && turns > expected)
	{
		write(STDERR_FILENO, MORE_ANTS, sizeof(MORE_ANTS) - 1);
		return (1);
	}
	else
		return (0);
}
