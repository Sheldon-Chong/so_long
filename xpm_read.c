#include "./libft/libft.h"
#include <sys/fcntl.h>
#include <stdio.h>

void test(char *file)
{
	int		fd;
	int		lines;
	int		i;
	char	*test;
	char	**array;

	lines = 0;;
	lines = 0;
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
		if(ft_strchr(test, ';') != 0 || !test)
			break;
		printf("%s\n",test);
		lines ++;
	}
}

int main()
{
	test("tile2.xpm");
}