/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 08:49:43 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/24 16:54:53 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reader.h"
#include "mem.h"

t_reader		io_create_reader(int fd)
{
	return ((t_reader) {
		.index = 0,
		.len = 0,
		.fd = fd
	});
}

static ssize_t	io_fill(t_reader *r)
{
	ssize_t	ret;

	r->index = 0;
	if ((ret = read(r->fd, r->buffer, sizeof(r->buffer))) < 0)
		return (ret);
	return (r->len = ret);
}

int16_t			io_peek(t_reader *r)
{
	if (r->index == r->len)
		if (io_fill(r) <= 0)
			return (-1);
	return (r->buffer[r->index]);
}

bool			io_expect(t_reader *r, const char *str)
{
	while (*str)
		if (io_peek(r) == *str)
		{
			r->index++;
			str++;
		}
		else
			return (false);
	return (true);
}

bool			io_readnum(t_reader *r, int32_t *num)
{
	int16_t		c;
	uint64_t	res;
	bool		sign;

	res = 0;
	sign = 0;
	if ((c = io_peek(r)) == '-' || c == '+')
	{
		sign = c == '-';
		r->index++;
	}
	while ((c = io_peek(r)) >= '0' && c <= '9')
	{
		res = res * 10 + (c - '0');
		if (res > ((uint64_t)INT32_MAX + sign))
			return (false);
		r->index++;
	}
	*num = sign ? -res : res;
	return (true);
}
