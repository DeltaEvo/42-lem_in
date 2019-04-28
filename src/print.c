#include "lem_in.h"
#include "mem.h"
#include <unistd.h>

void	unmark(struct s_room *room)
{
	size_t	i;

	room->mark = false;
	room->broken = false;
	room->prev = NULL;
	i = 0;
	while (i < room->links->len)
	{
		if (room->links->rooms[i].ptr->mark)
			unmark(room->links->rooms[i].ptr);
		i++;
	}
}

void	print_comments(const char *comments)
{
	const char	*end;

	if (!comments)
		return ;
	while (*comments)
	{
		end = ft_strchr(comments, '\n');
		if (!end)
			end = comments + ft_strlen(comments);
		write(STDOUT_FILENO, "#", 1);
		write(STDOUT_FILENO, comments, end - comments);
		write(STDOUT_FILENO, "\n", 1);
		comments = end + 1;
	}
}

void	print_room(struct s_room *room)
{
	size_t	i;

	room->mark = true;
	print_comments(room->comments);
	printf("%s %d %d\n", room->name, room->x, room->y);
	fflush(stdout);
	i = 0;
	while (i < room->links->len)
	{
		if (!room->links->rooms[i].ptr->mark)
			print_room(room->links->rooms[i].ptr);
		i++;
	}
}

void	print_links(struct s_room *room)
{
	size_t	i;

	room->mark = true;
	i = 0;
	while (i < room->links->len)
	{
		if (!room->links->rooms[i].ptr->mark)
		{
			print_comments(room->links->rooms[i].comments);
			write(STDOUT_FILENO, room->name, ft_strlen(room->name));
			write(STDOUT_FILENO, "-", 1);
			write(STDOUT_FILENO, room->links->rooms[i].ptr->name, ft_strlen(room->links->rooms[i].ptr->name));
			write(STDOUT_FILENO, "\n", 1);
		}
		i++;
	}
	i = 0;
	while (i < room->links->len)
	{
		if (!room->links->rooms[i].ptr->mark)
			print_links(room->links->rooms[i].ptr);
		i++;
	}
}

void	print_anthil(struct s_anthil anthil)
{
	print_comments(anthil.start_comments);
	printf("%zu\n", anthil.ants);
	fflush(stdout);
	print_room(anthil.start);
	unmark(anthil.start);
	write(STDOUT_FILENO, "\n", 1);
	print_links(anthil.start);
	unmark(anthil.start);
	print_comments(anthil.end_comments);
}
