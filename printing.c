/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:47:35 by shechong          #+#    #+#             */
/*   Updated: 2024/01/25 10:07:42 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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
