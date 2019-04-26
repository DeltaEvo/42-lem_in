/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 08:28:35 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/04/26 15:31:39 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reader.h"
#include "lem_in.h"

struct s_anthil read_anthil(t_reader *r);

int	main(void)
{
	t_reader		r;
	struct s_anthil	anthil;

	r = io_create_reader(0);
	anthil = read_anthil(&r);
	if (!anthil.start)
	{
		error("No start link\n");
		return (1);
	}
	if (anthil.start->end)
	{
		error("Start is end\n");
		return (1);
	}
	printf("Ants: %zu\n", anthil.ants);
}
