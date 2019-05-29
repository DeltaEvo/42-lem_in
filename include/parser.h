/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 17:21:13 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/29 05:57:52 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lem_in.h"
# include "reader.h"
# include "hashtable.h"

bool	has_command(const char *comments, const char *command);
char	*read_comments(t_reader *r);
char	*read_name(t_reader *r);
void	skip_nl(t_reader *r);
void	skip_ws(t_reader *r);
bool	free_and_warn_unused(struct s_hashtable *hashtable);
bool	read_anthil(t_reader *r, struct s_anthil *anthil);

#endif
