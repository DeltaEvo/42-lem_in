/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 01:58:31 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 05:24:45 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "mem.h"
#include <stdlib.h>

void	*error_malloc(void *ptr)
{
	write(STDERR_FILENO, ERROR_MALLOC, sizeof(ERROR_MALLOC) - 1);
	if (ptr)
		free(ptr);
	return (NULL);
}

bool	error_too_much_data(void)
{
	write(STDERR_FILENO, ERROR_TOO_MUCH, sizeof(ERROR_TOO_MUCH) - 1);
	return (true);
}

bool	error_invalid_ants(void)
{
	write(STDERR_FILENO, ERROR_INVALID_ANTS, sizeof(ERROR_INVALID_ANTS) - 1);
	return (true);
}

bool	error_duplicate(char *name)
{
	write(STDERR_FILENO, ERROR_DUPLICATE, sizeof(ERROR_DUPLICATE) - 1);
	write(STDERR_FILENO, name, ft_strlen(name));
	write(STDERR_FILENO, "\"\n", 2);
	return (true);
}

bool	error_illegal(char *name)
{
	write(STDERR_FILENO, ERROR_ILLEGAL_PRE, sizeof(ERROR_ILLEGAL_PRE) - 1);
	write(STDERR_FILENO, name, ft_strlen(name));
	write(STDERR_FILENO, ERROR_ILLEGAL_POST, sizeof(ERROR_ILLEGAL_POST) - 1);
	return (true);
}
