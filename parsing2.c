#include "graphics.h"

void	recur(char **c, t_xy pos, t_world *world, int *exit_found)
{
	if (!(pos.x < world->dimensions.x && pos.y < world->dimensions.y
			&& pos.x >= 0 && pos.y >= 0
			&& c[pos.y][pos.x] != '1' && c[pos.y][pos.x] != 'L'))
		return ;
	if (c[pos.y][pos.x] == 'E')
		*exit_found = 1;
	c[pos.y][pos.x] = 'L';
	recur(c, (t_xy){pos.x, pos.y + 1}, world, exit_found);
	recur(c, (t_xy){pos.x, pos.y - 1}, world, exit_found);
	recur(c, (t_xy){pos.x + 1, pos.y}, world, exit_found);
	recur(c, (t_xy){pos.x - 1, pos.y}, world, exit_found);
}

char	**clone_char_array(char **c)
{
	int		y;
	char	**ret_array;

	y = 0;
	while (c[y])
		y ++;
	ret_array = malloc(sizeof(char *) * (y + 1));
	y = -1;
	while (c[++y])
		ret_array[y] = ft_strdup(c[y]);
	ret_array[y] = NULL;
	return (ret_array);
}

int	find_exit(char **c, t_world *world)
{
	int		exit_found;
	char	**dup;

	exit_found = 0;
	dup = clone_char_array(c);
	recur(dup, world->player->pos, world, &exit_found);
	ft_free_array((void **)dup, 0);
	return (exit_found);
}
