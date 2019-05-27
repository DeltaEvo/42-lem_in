#include "lem_in.h"

bool	ffree(void *ptr)
{
	free(ptr);
	return (false);
}

void	free_links(struct s_link_vec *links)
{
	size_t	i;

	i = 0;
	while (i < links->len)
	{
		if (links->elems[i].first)
			free(links->elems[i].comments);
		i++;
	}
	free(links);
}

void	free_paths(struct s_path_vec *paths)
{
	size_t	i;

	i = 0;
	while (i < paths->len)
	{
		free(paths->paths[i].path);
		i++;
	}
	free(paths);
}

bool	free_room(struct s_room *room)
{
	free(room->name);
	free(room->comments);
	free_links(room->in.links);
	free_links(room->out.links);
	free(room);
	return (false);
}

void	free_anthil(struct s_anthil *anthil)
{
	size_t	i;

	i = 0;
	while (i < anthil->rooms->len)
	{
		free_room(anthil->rooms->rooms[i]);
		i++;
	}
	free(anthil->rooms);
	free(anthil->start_comments);
	free(anthil->end_comments);
	if (anthil->paths)
		free_paths(anthil->paths);
}
