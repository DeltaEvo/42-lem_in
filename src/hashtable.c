/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 13:58:43 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/24 17:44:16 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"
#include "mem.h"
#include <stdlib.h>
#include <stdbool.h>

static uint64_t			hash(const char *str)
{
	uint64_t value;

	value = 456;
	while (*str)
	{
		value = value * 31 + *str;
		str++;
	}
	return (value);
}

struct s_entry			create_entry(char *key)
{
	return ((struct s_entry) {
		.key = key,
		.hash = hash(key)
	});
}

struct s_hashtable		*create_hashtable(size_t size)
{
	struct s_hashtable	*hash;
	size_t				i;

	if (!(hash = malloc(sizeof(*hash) + size * sizeof(*hash->bucket))))
		return (NULL);
	i = 0;
	while (i < size)
	{
		hash->bucket[i].key = NULL;
		i++;
	}
	hash->size = size;
	return (hash);
}

static bool				grow_hashtable(struct s_hashtable **old)
{
	struct s_hashtable	*table;
	size_t				i;
	struct s_entry		*entry;

	if (!(table = create_hashtable((*old)->size * 2)))
		return (false);
	i = 0;
	while (i < (*old)->size)
	{
		entry = (*old)->bucket + i;
		if (entry->key)
			hashtable_insert(&table, *entry);
		i++;
	}
	free(*old);
	*old = table;
	return (true);
}

struct s_entry			*hashtable_insert(struct s_hashtable **table,
		struct s_entry entry)
{
	size_t					i;
	size_t					j;
	const struct s_entry	*curr = (*table)->bucket;

	j = 0;
	i = entry.hash % (*table)->size;
	while (curr[i % (*table)->size].key != NULL
			&& j < (*table)->size / 2)
	{
		if (entry.hash == curr[i % (*table)->size].hash)
			if (ft_strcmp(entry.key, curr[i % (*table)->size].key) == 0)
				return (NULL);
		i++;
		j++;
	}
	if (j == (*table)->size / 2)
	{
		if (!grow_hashtable(table))
			return (NULL);
		return (hashtable_insert(table, entry));
	}
	(*table)->bucket[i % (*table)->size] = entry;
	return ((*table)->bucket + (i % (*table)->size));
}

struct s_entry			*hashtable_get(struct s_hashtable *table,
		const char *name)
{
	uint64_t				hash_name;
	size_t					i;
	size_t					j;
	struct s_entry *const	curr = table->bucket;

	hash_name = hash(name);
	i = hash_name % table->size;
	j = 0;
	while (j < table->size
			&& curr[i % table->size].key
			&& (curr[i % table->size].hash != hash_name
				|| ft_strcmp(curr[i % table->size].key, name) != 0))
	{
		i++;
		j++;
	}
	if (curr[i % table->size].key && j < table->size)
		return (curr + i % table->size);
	else
		return (NULL);
}
