/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:47:09 by shechong          #+#    #+#             */
/*   Updated: 2023/12/14 17:25:15 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "graphics.h"
#include <fcntl.h>

int	shut(void *param)
{
	t_world		*world;
	t_display	*display;

	world = ((t_frame *)(param))->world;
	display = ((t_frame *)(param))->display;
	endgame(world, display);
	return (1);
}

int	get_color(t_img *img, int x, int y)
{
	char	*src;

	if (x < 0 || y < 0)
		return (0);
	src = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	return (*(unsigned int *)src);
}

int img_impose(t_img *canvas, t_img *img, t_xy start, t_xy scaling)
{
	t_xy pix;
	int color;

	pix = (t_xy){-1,-1};
	scaling.x = img->width * scaling.x;
	scaling.y = img->height * scaling.y;
	while (++pix.y < scaling.y)
	{
		pix.x = -1;
		while (++pix.x < scaling.x)
		{
			
			if (start.x + pix.x > 0 && start.y + pix.y > 0)
			{
				if(pix.x + start.x > SCREEN_WIDTH || pix.y + start.y > SCREEN_HEIGHT)
					break;
				if (scaling.x == img->width && scaling.y == img->height)
					color = get_color(img, pix.x, pix.y);
				else
					color = get_color(img, (int)(((double)pix.x / (double)scaling.x) * img->width), (int)(((double)pix.y / (double)scaling.y) * img->height));
				if(color != -16777216 && color != 0xFFFF0000)
					put_pixel(canvas, pix.x + start.x, pix.y + start.y, color);
			}
		}
	}
	return 1;
}

int clear_img(t_img *img)
{
	t_xy pix;

	pix = (t_xy){-1,-1};
	while(++pix.y < img->height)
	{
		pix.x = -1;
		while(++pix.x < img->width)
			put_pixel(img, pix.x, pix.y, COLOR_BLACK);	
	}
	return 1;
}

int	main(void)
{
	t_display	display;
	t_world		*world;
	t_frame		frame;
	int			frame_sec;

	display = display_init(SCREEN_WIDTH, SCREEN_HEIGHT);
	world = world_init("map.ber");
	world->coins = NULL;
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
