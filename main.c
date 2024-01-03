/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:47:09 by shechong          #+#    #+#             */
/*   Updated: 2024/01/03 15:15:17 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "graphics.h"
#include <fcntl.h>


int	clear_img(t_img *img)
{
	t_xy	pix;

	pix = (t_xy){-1, -1};
	while (++pix.y < img->height)
	{
		pix.x = -1;
		while (++pix.x < img->width)
			put_pixel(img, pix.x, pix.y, COLOR_BLACK);
	}
	return (1);
}

int	main(void)
{
	t_display	display;
	t_world		*world;
	t_frame		frame;
	int			frame_sec;

	display = display_init(SCREEN_WIDTH, SCREEN_HEIGHT);
	world = world_init("map.ber");
	world->collectibles = NULL;
	world->enemies = NULL;
	animation_init(world, &display);
	world->tgrid = char2tile(world, count_newline("map.ber"), &display);
	if (find_exit(world->grid, world) == 0)
		exit(write(2, "Error: Player cannot reach exit\n", 32));
	display.img = empty_img(display.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	frame_sec = 0;
	frame = (t_frame){&frame_sec, world, &display};
	mlx_hook(display.mlx_win, 2, 1L << 0, handle_keypress, &frame);
	mlx_loop_hook(display.mlx, render_next_frame, &frame);
	mlx_hook(display.mlx_win, 6, 1L << 6, mouse, &frame);
	mlx_hook(display.mlx_win, ON_DESTROY, 0, shut, &frame);
	mlx_loop(display.mlx);
}
