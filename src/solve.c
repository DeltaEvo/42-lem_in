#include "queue.h"

void	unmark(struct s_room *room);

struct s_room	*find_path(struct s_anthil *anthil)
{
	struct s_queue	*queue;
	struct s_room	*room;
	size_t			i;

	queue = create_queue(10);
	*queue_push(&queue) = anthil->start;
	anthil->start->mark = true;
	while (!queue_empty(queue))
	{
		room = queue_pop(queue);
		if (room->end)
			return (room);
		if (!room->old_prev || room->old_prev->broken)
		{
			i = 0;
			while (i < room->links->len)
			{
				if (!room->links->rooms[i].ptr->mark
					&& room->links->rooms[i].ptr->old_prev != room)
				{
					room->links->rooms[i].ptr->mark = true;
					room->links->rooms[i].ptr->prev = room;
					*queue_push(&queue) = room->links->rooms[i].ptr;
				}
				i++;
			}
		}
		else if (!room->old_prev->mark)
		{
			room->old_prev->prev = room;
			room->old_prev->mark = true;
			room->old_prev->broken = true;
			*queue_push(&queue) = room->old_prev;
		}
	}
	return (NULL);
}

void	print_path(struct s_room *room, bool last)
{
	if (room->prev)
		print_path(room->prev, false);
	printf("%s", room->name);
	if (last)
		printf("\n");
	else
		printf("->");
}

void	find_all_paths(struct s_anthil *anthil)
{
	struct s_room	*room;

	while ((room = find_path(anthil)))
	{
		print_path(room, true);
		while (room != anthil->start)
		{
			if (!room->broken)
				room->old_prev = room->prev;
			room = room->prev;
		}
		unmark(anthil->start);
	}
}
