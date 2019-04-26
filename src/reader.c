/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 08:49:43 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/04/26 15:46:36 by dde-jesu         ###   ########.fr       */
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

ssize_t			io_read(t_reader *r, char data[], size_t len)
{
	size_t	copied;
	size_t	remaining;
	ssize_t	ret;

	remaining = r->len - r->index;
	copied = 0;
	while (remaining < len)
	{
		copied += remaining;
		ft_memcpy(data, r->buffer + r->index, remaining);
		if ((ret = io_fill(r)) <= 0)
			return (ret < 0 ? ret : copied);
		data += remaining;
		len -= remaining;
		remaining = r->len - r->index;
	}
	ft_memcpy(data, r->buffer + r->index, len);
	r->index += len;
	return (len + copied);
}

int16_t			io_peek(t_reader *r)
{
	if (r->index == r->len)
		if (io_fill(r) <= 0)
			return (-1);
	return (r->buffer[r->index]);
}

bool		io_expect(t_reader *r, const char *str)
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

bool		io_skip_until(t_reader *r, char e)
{
	int16_t c;

	while ((c = io_peek(r)) != -1 && c != e )
		r->index++;
	if (c == -1)
		return (false);
	else
	{
		r->index++;
		return (true);
	}
}

bool		io_readnum(t_reader *r, int32_t *num)
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

