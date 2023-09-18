/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:47:09 by shechong          #+#    #+#             */
/*   Updated: 2023/09/18 11:40:58 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "graphics.h"
#include <fcntl.h>

void	ft_free_objects(t_object **head)
{
	t_object	*current;
	t_object	*next;

	current = *head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

void	free_display(t_display *display)
{
	free(display->camera);

	mlx_destroy_image(display->mlx, display->sprites[1]);
	mlx_destroy_image(display->mlx, display->sprites[2]);
	mlx_destroy_image(display->mlx, display->sprites[3]);
	mlx_destroy_image(display->mlx, display->sprites[4]);
	
	free(display->animations);
	free(display->anim_spritesheet);

	free(display->sprites[5]);
	free(display->sprites);
	free(display->grid_display);
}

void	free_world(t_world *world)
{
	free(world->player);
	free(world->enemies);
	free(world->coins);
	int	i;

	i = -1;
	while(world->grid[++i])
		free(world->grid[i]);
	free(world->grid[i]);
	free(world->grid);
	
	i = -1;
	while(world->tgrid[++i])
		free(world->tgrid[i]);
	free(world->tgrid[i]);
	free(world->tgrid);
}

void	free_ojects(t_world *world, t_display *display)
{
	t_object	*next;
	t_object	*current;

	current = world->enemies;
	while (current)
	{
		next = current->next;
		free(((t_enemy *)(current->data))->timers);
		free(((t_enemy *)(current->data))->timers->data);
		free(current);
		current = next;
	}
	free(current);
	current = world->coins;
	while (current)
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}
	free(current);
	current = display->animations;
}

void	free_animations(t_world *world, t_display *display)
{
	t_object	*current;
	t_object	*next;

	current = display->animations;
	
	while (current)
	{
		printf("lol2\n");
		next = current->next;
		mlx_destroy_image(display->mlx, ((t_animator *)(current->data))->frames[0]);
		mlx_destroy_image(display->mlx, ((t_animator *)(current->data))->frames[1]);
		free(((t_animator *)(current->data))->frames[3]);
		free(((t_animator *)(current->data))->frames);
		free(current->data);
		free(current);
		current = next;
	}
	free(display->animations);
	current = display->anim_spritesheet;
	while (current)
	{
		printf("lol\n");
		next = current->next;
		mlx_destroy_image(display->mlx, ((t_animator *)(current->data))->frames[0]);
		mlx_destroy_image(display->mlx, ((t_animator *)(current->data))->frames[1]);
		free(((t_animator *)(current->data))->frames[3]);
		free(((t_animator *)(current->data))->frames);
		free(current->data);
		free(current);
		current = next;
	}
	free(display->anim_spritesheet);
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