/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:47:29 by shechong          #+#    #+#             */
/*   Updated: 2024/01/25 10:07:42 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	find_holes(char **array, int rows)
{
	int	i;

	i = -1;
	if (!array || !array[0] || !array[0][0])
		return (0);
	while (array[0][++i])
		pass(array[0][i] != '1' || array[rows - 1][i] != '1',
			"Error: Not fully walled\n");
	i = -1;
	while (array[++i])
		pass(array[i][0] != '1' || array[i][ft_strlen(array[0]) - 1] != '1',
			"Error: Not fully walled\n");
	return (0);
}

void	count_items(char *array, t_world *world)
{
	int	i;

	i = -1;
	while (array[++i])
	{
		if (array[i] == 'P')
			world->count.player ++;
		if (array[i] == 'C')
			world->count.collectible ++;
		if (array[i] == 'E')
			world->count.exit ++;
	}
}

t_tile	**char2tile(t_world *world, int row_count, t_display *display)
{
	t_xy	pos;
	t_tile	**ret_array;

	ret_array = malloc(sizeof(t_tile *) * (row_count + 1));
	pos.y = -1;
	while (++pos.y < row_count)
	{
		ret_array[pos.y] = malloc(16 * (ft_strlen(world->grid[0]) + 1));
		pos.x = -1;
		while (++pos.x < (int)(ft_strlen(world->grid[pos.y])))
		{
			ret_array[pos.y][pos.x] = (t_tile){world->grid[pos.y][pos.x], NULL};
			if (world->grid[pos.y][pos.x] == 'S')
				ret_array[pos.y][pos.x].data = append(&world->enemies, new_obj(\
					"enemy", new_sentry(display, (t_xy){pos.x, pos.y})))->data;
			if (world->grid[pos.y][pos.x] == 'C')
				ret_array[pos.y][pos.x].data = append(&world->collectibles, \
	new_obj("c", new_collectible(display, (t_xy){pos.x, pos.y})))->data;
			if (world->grid[pos.y][pos.x] == 'P')
				world->player->pos = (t_xy){pos.x, pos.y};
		}
		ret_array[pos.y][pos.x] = (t_tile){0, NULL};
	}
	ret_array[pos.y] = NULL;
	return (ret_array);
}

char	**scan_map(t_world *world, char *file)
{
	int		line_count;
	char	**char_array;
	int		fd;

	pass(ft_strncmp(file + ft_strlen(file - 4), ".ber", 4),
		"Not a .ber file\n");
	fd = open(file, 0);
	pass(fd == -1, "Error: Cannot read file\n");
	line_count = count_newline(file, world);
	pass(world->count.player != 1, "Error: Incorrect number of players\n");
	pass(world->count.exit < 1, "Error: No exit present\n");
	pass(world->count.collectible < 1, "Error: No collectible present\n");
	pass(line_count < 3, "Error: Incorrect length\n");
	char_array = map2char_array(file, line_count, world);
	find_holes(char_array, line_count);
	world->dimensions.y = line_count;
	world->dimensions.x = ft_strlen(char_array[0]);
	return (char_array);
}

char	**map2char_array(char *file, int rows, t_world *world)
{
	int		i;
	char	*buffer;
	char	**array;
	int		fd;

	fd = open(file, 0);
	i = 0;
	buffer = get_next_line(fd);
	array = malloc(sizeof(char *) * (rows + 1));
	while (buffer)
	{
		array[i++] = ft_substr(buffer, 0, ft_strrchr(buffer, '\n') - buffer);
		free(buffer);
		buffer = get_next_line(fd);
	}
	array[i] = NULL;
	close(fd);
	return (array);
}
