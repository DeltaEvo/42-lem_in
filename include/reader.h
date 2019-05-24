/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 08:49:09 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/24 16:52:57 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READER_H
# define READER_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <unistd.h>

# define BUFFER_SIZE 2048

typedef struct	s_reader
{
	uint8_t		buffer[BUFFER_SIZE];
	size_t		len;
	size_t		index;
	int			fd;
}				t_reader;

t_reader		io_create_reader(int fd);
int16_t			io_peek(t_reader *r);
bool			io_expect(t_reader *r, const char *str);
//bool			io_skip_until(t_reader *r, char e);
bool			io_readnum(t_reader *r, int32_t *num);
#endif
