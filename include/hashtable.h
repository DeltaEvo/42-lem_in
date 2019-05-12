/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 11:47:52 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/05/12 13:39:29 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHTABLE_H
# define HASHTABLE_H

# include <stdint.h>
# include <stddef.h>

struct s_entry {
	char		*key;
	uint64_t	hash;
	void		*value;	
};

struct s_hashtable {
	size_t			size;
	struct s_entry	bucket[];
};

struct s_entry			*hashtable_get(struct s_hashtable *table, const char *name);
struct s_entry			*hashtable_insert(struct s_hashtable **table,
		struct s_entry entry);
struct s_hashtable		*create_hashtable(size_t size);
struct s_entry			create_entry(char *key);

#endif
