/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 14:03:25 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/26 18:04:53 by dde-jesu         ###   ########.fr       */
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
	struct s_node	*nodes[];
};

struct s_queue	*create_queue(size_t size);
bool			queue_empty(struct s_queue *queue);
struct s_node	**queue_push(struct s_queue **queue);
struct s_node	*queue_pop(struct s_queue *queue);

#endif
