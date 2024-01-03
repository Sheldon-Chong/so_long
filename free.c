/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:52:14 by shechong          #+#    #+#             */
/*   Updated: 2024/01/03 15:13:23 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

void	img_destroy(void *mlx, t_img *img)
{
	mlx_destroy_image(mlx, img->img);
	free(img);
}

void	free_world_display(t_world *world, t_display *display)
{
	int	i;

	free(world->player);
	i = -1;
	while (world->grid[++i])
		free(world->grid[i]);
	free(world->grid[i]);
	free(world->grid);
	i = -1;
	while (world->tgrid[++i])
		free(world->tgrid[i]);
	free(world->tgrid[i]);
	free(world->tgrid);

	free(display->camera);
	img_destroy(display->mlx, display->sprites[1]);
	img_destroy(display->mlx, display->sprites[2]);
	img_destroy(display->mlx, display->sprites[3]);
	img_destroy(display->mlx, display->sprites[4]);
	free(display->sprites);
	free(display->grid_display);
}

void	free_ojects(t_world *world, t_display *display)
{
	t_object	*next;
	t_object	*current;

	current = world->enemies;
	while (current)
	{
		next = current->next;
		img_destroy(display->mlx,
			((t_sentry *)(current->data))->animator.frames[0]);
		free(((t_sentry *)(current->data))->animator.frames);
		free(current->data);
		free(current);
		current = next;
	}
}

void	free_collectibles(t_world *world, t_display *display)
{
	t_object	*next;
	t_object	*current;

	current = world->collectibles;
	while (current)
	{
		next = current->next;
		img_destroy(display->mlx,
			((t_collectible *)(current->data))->animator.frames[0]);
		img_destroy(display->mlx,
			((t_collectible *)(current->data))->animator.frames[1]);
		free(((t_collectible *)(current->data))->animator.frames);
		free(current->data);
		free(current);
		current = next;
	}
}

int	endgame(t_world *world, t_display *display)
{
	free_ojects(world, display);
	free_collectibles(world, display);
	img_destroy(display->mlx, world->player->animator.frames[0]);
	img_destroy(display->mlx, world->player->animator.frames[1]);
	free(world->player->animator.frames);
	free_world_display(world, display);
	free(world);
	exit(0);
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
