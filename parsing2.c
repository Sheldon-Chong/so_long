/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 10:53:58 by shechong          #+#    #+#             */
/*   Updated: 2024/01/25 10:07:42 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	recur(char **c, t_xy pos, t_world *world, int *exit_found)
{
	int	collectible_count;

	collectible_count = 0;
	if (pos.x >= world->dimensions.x - 1 || pos.y >= world->dimensions.y -1
		|| pos.x < 0 || pos.y < 0 || c[pos.y][pos.x] == '1'
		|| c[pos.y][pos.x] == 'L')
		return (collectible_count);
	if (c[pos.y][pos.x] == 'E')
		*exit_found = 1;
	if (c[pos.y][pos.x] == 'C')
		collectible_count ++;
	c[pos.y][pos.x] = 'L';
	collectible_count += recur(c, (t_xy){pos.x, pos.y + 1}, world, exit_found);
	collectible_count += recur(c, (t_xy){pos.x, pos.y - 1}, world, exit_found);
	collectible_count += recur(c, (t_xy){pos.x + 1, pos.y}, world, exit_found);
	collectible_count += recur(c, (t_xy){pos.x - 1, pos.y}, world, exit_found);

	return (collectible_count);
}

char	**dup_char_array(char **c)
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
	int		exit_reachable;
	char	**dup;
	int		collectible_count;

	exit_reachable = 0;
	dup = dup_char_array(c);
	collectible_count = recur(dup, world->player->pos, world, &exit_reachable);

	if (collectible_count != world->count.collectible)
		exit(write(2, "Error: Not all collectibles are reachable\n", 43));
	if (!exit_reachable)
		exit(write(2, "Error: Player cannot reach exit\n", 32));
	ft_free_array((void **)dup, 0);
	return (exit_reachable);
}

int	count_newline(char *filename, t_world *world)
{
	char	*buffer;
	int		fd;
	int		newline_count;
	int		prev_length;

	newline_count = 0;
	fd = open(filename, 0);
	buffer = get_next_line(fd);
	prev_length = ft_strlen(buffer);
	while (buffer)
	{
		newline_count++;
		count_items(buffer, world);
		pass(ft_is_charset(buffer, "1P2CESH0\n") != 1,
			"Error: Invalid character found\n");
		pass(prev_length != ft_strlen(buffer),
			"Error: Map is not rectangular\n");
		pass(buffer[0] != '1' || buffer[ft_strlen(buffer) - 1] != '1',
			"Error: Not fully walled\n");
		prev_length = ft_strlen(buffer);
		free(buffer);
		buffer = get_next_line(fd);
	}
	close(fd);
	return (newline_count);
}
