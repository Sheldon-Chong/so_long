/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:52:14 by shechong          #+#    #+#             */
/*   Updated: 2023/09/18 11:42:26 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"



void	free_grid(t_world *world)
{
	int	i;

	i = 0;
	while (world->tgrid[i])
	{
		free(world->tgrid[i]);
		i ++;
	}
	free(world->tgrid);
	i = 0;
	ft_free_array((void **)(world->grid), 0);
	free(world->grid);
}

int	endgame(t_world *world, t_display *display)
{
	t_object	*current;
	t_object	*next;

	mlx_destroy_window(display->mlx, display->mlx_win);
	print_end_screen(world);
	exit(0);
	current = display->animations;
	while (current)
	{
		next = current->next;
		ft_free_array((void *)(((t_animator *)(current->data))->frames), 0);
		current = next;
	}
	current = display->anim_spritesheet;
	while (current)
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}	
	free(display->sprites);
	free(display->camera);
	free(world);
	free(display);

	//system("leaks program");
	exit(0);
}

/*
printf("%lu t_animator\n", sizeof(t_animator));
	printf("%lu t_camera\n", sizeof(t_camera));
	printf("%lu t_coin\n", sizeof(t_coin));
	printf("%lu t_counter\n", sizeof(t_counter));
	printf("%lu t_data\n", sizeof(t_data));
	printf("%lu t_display\n", sizeof(t_display));
	printf("%lu t_enemy\n", sizeof(t_enemy));
	printf("%lu t_frame\n", sizeof(t_frame));
	printf("%lu t_list\n", sizeof(t_list));
	printf("%lu t_object\n", sizeof(t_object));
	printf("%lu t_player\n", sizeof(t_player));
	printf("%lu t_ray\n", sizeof(t_ray));
	printf("%lu t_tile\n", sizeof(t_tile));
	printf("%lu t_timer\n", sizeof(t_timer));
	printf("%lu t_world\n", sizeof(t_world));
	printf("%lu t_xy\n", sizeof(t_xy));
	*/