#include "./libft/libft.h"
#include <sys/fcntl.h>
#include <stdio.h>

void xpm_to_array(char *file, char **array)
{
	int		fd;
	int		lines;
	int		i;
	char	*test;

	lines = 0;;
	lines = 0;
	i = 0;
	fd = open(file, 0);
	test = get_next_line(fd);
	while(test && ft_strncmp(test, "/* pixels */", 11) != 0)
	{
		test = get_next_line(fd);
		lines ++;
	}
	while(test)
	{
		test = get_next_line(fd);
		test = ft_substr(test, 1, ft_strlen(test) - 4);
		array[i] = test;
		if(ft_strchr(test, ';') != 0 || !test)
			break;
		lines ++;
		i ++;
	}
	array[i] = NULL;
}

int	count_lines(char *file)
{
	int		fd;
	int		lines;
	char	*test;

	lines = 0;
	fd = open(file, 0);
	test = get_next_line(fd);
	while(test && ft_strncmp(test, "/* pixels */", 11) != 0)
		test = get_next_line(fd);
	while(test)
	{
		test = get_next_line(fd);
		lines ++;
	}
	close(fd);
	return lines;
}

int main()
{
	int size = count_lines("tile2.xpm");
	printf("%d", size);

	char **array = malloc((sizeof(char *) * size) + 1);

	xpm_to_array("tile2.xpm", array);

	int i = -1;
	while(array[++i])
		printf("%s\n", array[i]);
}