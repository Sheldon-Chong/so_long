#include "graphics.h"

void	print_2d_tiles(t_tile **c)
{
	int	y;
	int	x;

	y = 0;
	x = -1;
	ft_putstr_fd("\n     ",1);
	while (c[0][++x].type)
	{
		ft_putchar_fd("123456789|"[y], 1);
		y = (y + 1) % 10;
	}
	ft_putstr_fd("\n\n",1);
	y = -1;
	while (c[++y])
	{
		x = -1;
		ft_putnbr_fd(y,1);
		if(y < 10)
			ft_putstr_fd(" ",1);
		ft_putstr_fd(" | ",1);
		while (c[y][++x].type)
			ft_putchar_fd(c[y][x].type, 1);
		ft_putchar_fd('\n', 1);
	}
}

void	print_statistics(t_world *world)
{
	t_object	*head;

	printf("\nENEMIES:\n");
	head = world->enemies;
	while (head)
	{
		printf("Position: (%d, %d)\n", ((t_enemy *)(head->data))->pos.x, ((t_enemy *)(head->data))->pos.y);
		printf("HP: (%d)\n", ((t_enemy *)(head->data))->hp);
		head = head->next;
	}
	printf("\nCOLLECtIBLES:\n");
	head = world->enemies;
	while(head)
	{
		printf("Position: (%d, %d)\n", ((t_coin *)(head->data))->pos.x, ((t_coin *)(head->data))->pos.y);
		head = head->next;
	}
}