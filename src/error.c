/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 01:58:31 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 02:48:22 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include <unistd.h>
#include <stdlib.h>

void	*error_malloc(void *ptr)
{
	write(STDERR_FILENO, ERROR_MALLOC, sizeof(ERROR_MALLOC) - 1);
	if (ptr)
		free(ptr);
	return (NULL);
}
