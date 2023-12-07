/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:47:35 by shechong          #+#    #+#             */
/*   Updated: 2023/11/02 20:10:41 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

void	print_2d_tiles(t_tile **c)
{
	int	y;
	int	x;

	y = 0;
	x = -1;
	ft_putstr_fd("\n     ", 1);
	while (c[0][++x].type)
	{
		ft_putchar_fd("123456789|"[y], 1);
		y = (y + 1) % 10;
	}
	ft_putstr_fd("\n\n", 1);
	y = -1;
	while (c[++y])
	{
		x = -1;
		ft_putnbr_fd(y, 1);
		if (y < 10)
			ft_putstr_fd(" ", 1);
		ft_putstr_fd(" | ", 1);
		while (c[y][++x].type)
			ft_putchar_fd(c[y][x].type, 1);
		ft_putchar_fd('\n', 1);
	}
}

// void	print_statistics(t_world *world)
// {
// 	t_object	*head;

// 	printf("\nENEMIES:\n");
// 	head = world->enemies;
// 	while (head)
// 	{
// 		printf("Position: (%d, %d)\n", ((t_enemy *)(head->data))->pos.x,
// 			((t_enemy *)(head->data))->pos.y);
// 		printf("HP: (%d)\n", ((t_enemy *)(head->data))->hp);
// 		head = head->next;
// 	}
// 	printf("\nCOLLECtIBLES:\n");
// 	head = world->enemies;
// 	while (head)
// 	{
// 		printf("Position: (%d, %d)\n", ((t_coin *)(head->data))->pos.x,
// 			((t_coin *)(head->data))->pos.y);
// 		head = head->next;
// 	}
// }

void	print_char_array(char **c)
{
	int	y;
	int	x;

	y = -1;
	while (c[++y])
	{
		x = -1;
		while (c[y][++x])
			ft_putchar_fd(c[y][x], 1);
		ft_putchar_fd('\n', 1);
	}
}

void	print_end_screen(t_world *world)
{
	ft_putstr_fd(" _______  _______  __   __  _______    ", 1);
	ft_putstr_fd("_______  __   __  _______  ______   \n", 1);
	ft_putstr_fd("|       ||   _   ||  |_|  ||       |  ", 1);
	ft_putstr_fd("|       ||  | |  ||       ||    _ |  \n", 1);
	ft_putstr_fd("|    ___||  |_|  ||       ||    ___|  ", 1);
	ft_putstr_fd("|   _   ||  |_|  ||    ___||   | ||  \n", 1);
	ft_putstr_fd("|   | __ |       ||       ||   |___   ", 1);
	ft_putstr_fd("|  | |  ||       ||   |___ |   |_||_ \n", 1);
	ft_putstr_fd("|   ||  ||       ||       ||    ___|  ", 1);
	ft_putstr_fd("|  |_|  ||       ||    ___||    __  |\n", 1);
	ft_putstr_fd("|   |_| ||   _   || ||_|| ||   |___   ", 1);
	ft_putstr_fd("|       | |     | |   |___ |   |  | |\n", 1);
	ft_putstr_fd("|_______||__| |__||_|   |_||_______|  ", 1);
	ft_putstr_fd("|_______|  |___|  |_______||___|  |_|\n", 1);
	ft_putstr_fd("|_______||__| |__||_|   |_||_______|  ", 1);
	ft_putstr_fd("|_______|  |___|  |_______||___|  |_|\n", 1);
	ft_putstr_fd("\n\nCoins collected:", 1);
	ft_putnbr_fd(world->count.collectible, 1);
	ft_putstr_fd("\n\n\n", 1);
}
