/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:47:09 by shechong          #+#    #+#             */
/*   Updated: 2023/11/02 19:32:20 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "graphics.h"
#include <fcntl.h>

void	print_list(t_object *start)
{
	t_object	*current;

	current = start;
	while (current)
	{
		printf("%s\n", current->type);
		current = current->next;
	}
}

int	shut(void *param)
{
	t_world		*world;
	t_display	*display;

	world = ((t_frame *)(param))->world;
	display = ((t_frame *)(param))->display;
	endgame(world, display);
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
	world->coins = NULL;
	world->enemies = NULL;
	animation_init(world, &display);
	world->tgrid = char2tile(world, count_newline("map.ber"), &display);
	if (find_exit(world->grid, world) == 0)
		exit(write(2, "Error: Player cannot reach exit\n", 32));
	frame_sec = 0;
	frame = (t_frame){&frame_sec, world, &display};
	mlx_hook(display.mlx_win, 2, 1L << 0, handle_keypress, &frame);
	mlx_loop_hook(display.mlx, render_next_frame, &frame);
	mlx_hook(display.mlx_win, 6, 1L << 6, mouse, &frame);
	mlx_hook(display.mlx_win, ON_DESTROY, 0, shut, &frame);
	mlx_loop(display.mlx);
}
