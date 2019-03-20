/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 08:28:35 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/20 11:31:38 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reader.h"
#include "lem_in.h"

struct s_anthil read_anthil(t_reader *r);

int	main(void)
{
	t_reader	r;

	r = io_create_reader(0);
	read_anthil(&r);
}
