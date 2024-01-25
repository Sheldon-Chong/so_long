#include "libft.h"

void	pass(int condition, char *str)
{
	if (condition)
	{
		ft_putstr_fd(str, 2);
		exit(1);
	}
}
