/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 08:28:35 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/20 08:31:21 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#define HELLO_WORLD "Hello World !\n"

int	main(void)
{
	write(1, HELLO_WORLD, sizeof(HELLO_WORLD) - 1);
}