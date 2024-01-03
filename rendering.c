/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:48:33 by shechong          #+#    #+#             */
/*   Updated: 2024/01/03 14:47:38 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

t_xy	render_tile(t_display *display,
	t_img *b_image, t_xy pos, t_xy mod)
{
	t_xy		ret;
	t_grid_d	*grid;

	grid = display->grid_display;
	ret = (t_xy){
		iso_map((t_xy){pos.x * grid->space_x,
			pos.y * grid->space_y}).x + grid->offset_x + mod.x,
		iso_map((t_xy){pos.x * grid->space_x,
			pos.y * grid->space_y}).y + grid->offset_y + mod.y};
	if (b_image == NULL)
		return (ret);
	img_impose(display->img, b_image, (t_xy){ret.x, ret.y}, (t_xy){1,1});
	return (ret);
}

int	render_floor(t_world *world, t_display *display, t_img **sprites)
{
	t_xy	tile;
	t_img	*b_image;
	t_tile	**tgrid;

	tgrid = world->tgrid;
	tile.y = -1;
	while (tgrid[++tile.y])
	{
		tile.x = -1;
		while (tgrid[tile.y][++tile.x].type)
		{	
			b_image = sprites[2];
			if ((tile.x % 2 == 0 && tile.y % 2 != 0)
				|| (tile.x % 2 != 0 && tile.y % 2 == 0))
				b_image = sprites[3];
			render_tile(display, b_image, (t_xy){tile.x, tile.y},
				(t_xy){0, (tgrid[tile.y][tile.x].type == '1') * -20});
		}
	}
	return (1);
}

void	render_obj(t_world *world, t_display *display, t_xy tile)
{
	t_tile		current_tile;
	t_img		**sprites;

	sprites = display->sprites;
	current_tile = world->tgrid[tile.y][tile.x];
	if (current_tile.type == '1')
		render_tile(display, sprites[1], (t_xy){tile.x, tile.y},
			(t_xy){0, (current_tile.type == '1') * -30});
	if (current_tile.type == 'E')
		render_tile(display, sprites[4],
			(t_xy){tile.x, tile.y}, (t_xy){0, -50});
	if (current_tile.type == 'S' && current_tile.data)
		render_sentry(display, (t_enemy *)((current_tile.data)));
	if (current_tile.type == 'C' && current_tile.data)
		render_tile(display, ((t_collectible *)(current_tile.data))->animator.current_frame, tile, (t_xy){center(display->sprites[1], 
			((t_collectible *)(current_tile.data))->animator.current_frame), -20});
	if (tile.x == world->player->pos.x && tile.y == world->player->pos.y)
		render_player(world, display);
}

void	render_world(t_world *world, t_display *display)
{
	t_xy		tile;

	render_floor(world, display, display->sprites);
	tile.y = -1;
	while (world->tgrid[++tile.y])
	{
		tile.x = -1;
		while (world->tgrid[tile.y][++tile.x].type)
			render_obj(world, display, tile);
	}
}

int	update_all_animators(t_display *display, t_world *world)
{
	t_object	*head;
	t_animator	*animator;

	update_enemies(world, display);
	head = world->coins;
	while(head)
	{
		animator = &((t_collectible *)(head->data))->animator;
		animator->frame_timer = (animator->frame_timer + 1) % animator->speed;
		if (animator->frame_timer == 0)
			animator->frame_index = (animator->frame_index + 1) % 2;
		animator->current_frame = animator->frames[animator->frame_index];
		head = head->next;
	}
	return (1);
}

void	place_pixel(t_img *image, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || y < 0)
		return ;
	dst = image->addr + (y * image->line_length + x * (image->bits_per_pixel
				/ 8));
	*(unsigned int *)dst = color;
}


