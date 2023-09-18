// #include "libft/libft.h"
// #include <sys/fcntl.h>
// #include <stdio.h>

// int main()
// {
// 	char	**test;
// 	int i = 0;
// 	char **array;

// 	int fd = open("map.ber", 0);

// 	char *buffer;

// 	buffer = get_next_line(fd);
// 	array = malloc(20 * sizeof(char *));
// 	while(buffer)
// 	{
// 		printf("%s", buffer);
// 		array[i] = ft_substr(buffer, 0, ft_strrchr(buffer, '\n') - buffer);
// 		printf("%s\n", array[i]);
// 		free(buffer);
// 		buffer = get_next_line(fd);
// 		printf("%s", buffer);
// 		i++;
// 	}
// }