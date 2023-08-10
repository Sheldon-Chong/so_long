#include "libft/libft.h"
#include <stdio.h>
#include <sys/fcntl.h>

// cant detect unusable characters. Might be the ft_is_charset function

int count_newline(char *filename)
{
	int fd = open("test.ber", 0);
	int i = 0;
	char *buffer;
	
	int newline_count = 0;
	int status;

	buffer = malloc(1);
	while ((status = read(fd, buffer, 1)) > 0)
		if (buffer[0] == '\n' || buffer[0] == '\r')
			newline_count++;
		if(ft_is_charset(buffer, "1P2CE") == 1)
		{	printf("invalid"); printf("<[%s]>", buffer);}
	if (status < 0)
		perror("Error reading file");
	
	free(buffer);
	return (newline_count + 1);
}

int find_holes(char **array, int rows)
{
	int i;

	i = -1;
	if(!array || !array[0])
		return(0);
	while(array[0][++i])
		if(array[0][i] != '1' || array[rows - 1][i] != '1')
			return(1);
	i = -1;
	while(array[++i])
		if(array[i][0] != '1' || array[i][ft_strlen(array[0]) - 1] != '1')
			return(1);
	return(0);
}

int main()
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
	buffer = get_next_line(fd);
	while(1)
	{
		if(!buffer)
			break;
		buffer = ft_substr(buffer, 0, ft_strrchr(buffer, '\n') - buffer);
		array[i] = buffer;
		i ++;
		buffer = get_next_line(fd);
	}
	array[i] = NULL;
	
	i = -1;
	while(array[++i + 1])
	 	if(ft_strlen(array[i]) != ft_strlen(array[i+1]))
			printf("incorrect length");

	printf("\n[%d]\n", find_holes(array, count));

	return (0);
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