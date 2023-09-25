/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:52:14 by shechong          #+#    #+#             */
/*   Updated: 2023/09/18 14:34:39 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"


void	free_display(t_display *display)
{
	free(display->camera);

	mlx_destroy_image(display->mlx, display->sprites[1]);
	mlx_destroy_image(display->mlx, display->sprites[2]);
	mlx_destroy_image(display->mlx, display->sprites[3]);
	mlx_destroy_image(display->mlx, display->sprites[4]);
	
	free(display->animations);
	free(display->anim_spritesheet);

	free(display->sprites);
	free(display->grid_display);
}

void	free_world(t_world *world)
{
	free(world->player);
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
		free(current->data);
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
	
	mlx_destroy_image(display->mlx, ((t_animator *)(display->anim_spritesheet->data))->frames[0]->img);
	mlx_destroy_image(display->mlx, ((t_animator *)(display->anim_spritesheet->data))->frames[1]->img);
	mlx_destroy_image(display->mlx, ((t_animator *)(display->anim_spritesheet->next->data))->frames[0]->img);
	mlx_destroy_image(display->mlx, ((t_animator *)(display->anim_spritesheet->next->data))->frames[1]->img);
	
	free(((t_animator *)(display->anim_spritesheet->data))->frames[0]);
	free(((t_animator *)(display->anim_spritesheet->data))->frames[1]);
	free(((t_animator *)(display->anim_spritesheet->next->data))->frames[0]);
	free(((t_animator *)(display->anim_spritesheet->next->data))->frames[1]);
	free(((t_animator *)(display->anim_spritesheet->data))->frames);
	free(((t_animator *)(display->anim_spritesheet->data)));

	mlx_destroy_image(display->mlx, ((t_animator *)(display->animations->next->next->data))->frames[0]->img);
	mlx_destroy_image(display->mlx, ((t_animator *)(display->animations->next->next->data))->frames[1]->img);
	free(((t_animator *)(display->animations->next->next->data))->frames[0]);
	free(((t_animator *)(display->animations->next->next->data))->frames[1]);
	free(((t_animator *)(display->animations->next->next->data))->frames);

	free(((t_animator *)(display->anim_spritesheet->next->data))->frames);
	free(((t_animator *)(display->anim_spritesheet->next->data)));

	free(display->anim_spritesheet->next->next);
	free(display->anim_spritesheet->next);

	free(display->animations->next->next);
	free(display->animations->next);
}

int	endgame(t_world *world, t_display *display)
{
	t_object	*current;
	t_object	*next;

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

	free_ojects(world,display); 
	free_animations(world, display);
	free_display(display);
	free_world(world);
	free(world);
	exit(0);
}
