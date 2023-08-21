#include "libft/libft.h"
#include <stdio.h>
#include <sys/fcntl.h>

// cant detect unusable characters. Might be the ft_is_charset function


int count_newline(char *filename)
{
	char *buffer;	
	int newline_count;
	int status;
	int fd;

	newline_count = 0;
	fd = open("test.ber", 0);
	buffer = malloc(2);
	while ((status = read(fd, buffer, 1)) > 0)
	{
		if (buffer[0] == '\n' || buffer[0] == '\r')
			newline_count++;
		buffer[1] = 0;
		if (ft_is_charset(buffer, "1P2CES0\n") != 1)
			exit(write(2, "Incorrect characters\n", 19));
	}
	if (status < 0)
		exit(write(2,"Error reading file\n",19));
	free(buffer);
	return (newline_count + 1);
}

int find_holes(char **array, int rows)
{
	int	i;

	i = -1;
	if(!array || !array[0])
		return(0);
	while(array[0][++i])
		if(array[0][i] != '1' || array[rows - 1][i] != '1')
			exit(write(2, "Not fully walled\n", 16));
	i = -1;
	while(array[++i])
		if(array[i][0] != '1' || array[i][ft_strlen(array[0]) - 1] != '1')
			exit(write(2, "Not fully walled\n", 16));
	return(0);
}

int check_objects(char **c)
{
	int	y;
	int exits;
	int player;
	int collectible;
	
	exits = 0;
	player = 0;
	collectible = 0;
	y = -1;
	while(c[++y])
	{
		if(ft_strchr(c[y], 'P') != 0)
			player = 1;
		if(ft_strchr(c[y], 'C') != 0)
			collectible = 1;
		if(ft_strchr(c[y], 'E') != 0)
			exits = 1;
	}
	if(player != 1)
		exit(write(2,"No player\n", 10));
	if(collectible != 1)
		exit(write(2,"No collectible\n", 15));
	if(exits != 1)
		exit(write(2,"No exit\n", 8));
	return(0);
}

void	print_2d_array(char **c)
{
	int	y;
	int	x;

	y = -1;
	while(c[++y])
	{
		x = -1;
		while(c[y][++x])
			ft_putchar_fd(c[y][x], 1);
		ft_putchar_fd('\n', 1);
	}
}

char	**read_array(char *file, int rows)
{
	int		i;
	char	*buffer;
	char **array;
	int fd;
	
	fd = open(file, 0);
	i = 0;
	buffer = get_next_line(fd);
	array = malloc(sizeof(char *) * (rows + 1));
	while(buffer)
	{
		buffer = ft_substr(buffer, 0, ft_strrchr(buffer, '\n') - buffer);
		array[i++] = buffer;
		buffer = get_next_line(fd);
	}
	array[i] = NULL;
	return(array);
}

char	**scan_map()
{
	int count;
	char **array;
	char *buffer;
	int i;
	int i2;

	int fd = open("test.ber", 0);
	count = count_newline("test.ber");
	array = malloc(sizeof(char *) * (count + 1));
	i = 0;
	array = read_array("test.ber", count);
	i = -1;
	while(array[++i + 1])
	 	if(ft_strlen(array[i]) != ft_strlen(array[i+1]))
			exit(write(2, "Incorrect length\n", 17));
	check_objects(array);
	find_holes(array, count);
	print_2d_array(array);
	return (array);
}

/*
i = -1;
	while(array[++i])
		printf("%c", array[i][0]);
	printf("\n");
	i = -1;
	while(array[++i])
		printf("%c", array[i][ft_strlen(array[0]) - 1]);
	printf("\n");
	i = -1;
	while(array[0][++i])
		printf("%c", array[0][i]);
	printf("\n");
	i = -1;
	while(array[count-1][++i])
		printf("%c", array[count-1][i]);
*/