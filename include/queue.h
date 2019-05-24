/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 14:03:25 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/24 16:39:02 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUEUE_H
# define QUEUE_H

# include "lem_in.h"

struct			s_queue {
	size_t			head;
	size_t			tail;
	bool			full;
	size_t			size;
	struct s_room	*rooms[];
};

struct s_queue	*create_queue(size_t size);
bool			queue_empty(struct s_queue *queue);
struct s_room	**queue_push(struct s_queue **queue);
struct s_room	*queue_pop(struct s_queue *queue);

#endif
