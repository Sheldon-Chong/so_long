/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 10:53:58 by shechong          #+#    #+#             */
/*   Updated: 2023/09/14 19:19:48 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	count_newline(char *filename)
{
	char	*buffer;	
	int		newline_count;
	int		status;
	int		fd;

	newline_count = 0;
	fd = open(filename, 0);
	buffer = malloc(2);
	status = read(fd, buffer, 1);
	while (status > 0)
	{
		if (buffer[0] == '\n' || buffer[0] == '\r')
			newline_count++;
		buffer[1] = 0;
		if (ft_is_charset(buffer, "1P2CESH0\n") != 1)
			exit(write(2, "Error: Incorrect characters\n", 19));
		status = read(fd, buffer, 1);
	}
	if (status < 0)
		exit(write(2, "Error: reading file\n", 20));
	free(buffer);
	return (newline_count + 1);
}
