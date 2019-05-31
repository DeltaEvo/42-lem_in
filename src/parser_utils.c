/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 05:53:57 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/31 09:59:43 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "str.h"
#include "mem.h"

void	skip_ws(t_reader *r)
{
	while (io_peek(r) == ' ' || io_peek(r) == '\t')
		r->index++;
}

void	skip_nl(t_reader *r)
{
	while (io_peek(r) == '\n')
		r->index++;
}

char	*read_name(t_reader *r)
{
	int16_t	c;
	t_str	str;

	if (!str_init(&str, 8))
		return (error_malloc(NULL));
	while ((c = io_peek(r)) != -1 && c != ' ' && c != '\t' && c != '-'
			&& c != '\n')
	{
		if (!str_append(&str, c))
			return (error_malloc(str.inner));
		r->index++;
	}
	if (!(str_append(&str, '\0')))
		return (error_malloc(str.inner));
	return (str.inner);
}

char	*read_comments(t_reader *r)
{
	int16_t	c;
	t_str	str;

	if (io_peek(r) != '#')
		return (NULL);
	if (!str_init(&str, 8))
		return (error_malloc(NULL));
	r->index++;
	while ((c = io_peek(r)) != -1)
	{
		if (!str_append(&str, c))
			return (error_malloc(str.inner));
		r->index++;
		if (c == '\n')
		{
			skip_nl(r);
			if (io_peek(r) == '#')
				r->index++;
			else
				break ;
		}
	}
	if (!(str_append(&str, '\0')))
		return (error_malloc(str.inner));
	return (str.inner);
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
