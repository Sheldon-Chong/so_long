/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:47:09 by shechong          #+#    #+#             */
/*   Updated: 2023/09/18 14:34:10 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "graphics.h"
#include <fcntl.h>

void	print_list(t_object *start)
{
	t_object *current = start;
	while(current)
	{
		printf("%s\n", current->type);
		current = current->next;
	}
}


int	main(void)
{
	t_display	display;
	t_world		*world;
	t_frame		frame;
	int			frame_sec;

	display = display_init(1920, 1080);
	world = world_init("map.ber");	
	world->coins = NULL;
	world->enemies = NULL;

	animation_init(world, &display);

	world->tgrid = char2tile(world, count_newline("map.ber"), &display);
	if (find_exit(world->grid, world) == 0)
		exit(write(2, "Error: Player cannot reach exit\n", 32));

	// free_ojects(world,&display); 
	// free_animations(world, &display);
	// free_display(&display);
	// free_world(world);
	// free(world);
	// exit(0);
	
	frame_sec = 0;
	frame = (t_frame){&frame_sec, world, &display};
	mlx_hook(display.mlx_win, 2, 1L << 0, handle_keypress, &frame);
	mlx_loop_hook(display.mlx, render_next_frame, &frame);
	mlx_hook(display.mlx_win, 6, 1L << 6, mouse, &frame);
	mlx_loop(display.mlx);
}


/*

free_animations(world, &display);

free_display(&display);
free_world(world);

free(world);

mlx_destroy_window(display.mlx, display.mlx_win);

exit(0);

*/