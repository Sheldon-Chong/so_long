#include "graphics.h"

t_xy	render_tile(t_display *display, t_grid_d grid, t_data *b_image, t_xy pos, t_xy mod)
{
	t_xy	ret;

	ret = (t_xy){
		iso_map((t_xy){pos.x * grid.space_x,
			pos.y * grid.space_y}).x + grid.offset_x + mod.x,
		iso_map((t_xy){pos.x * grid.space_x,
			pos.y * grid.space_y}).y + grid.offset_y + mod.y};
	if (b_image == NULL)
		return (ret);
	mlx_put_image_to_window(display->mlx, display->mlx_win,
		b_image->img, ret.x, ret.y);
	return (ret);
}

int	render_player(t_world *world, t_grid_d grid, t_display *display)
{
	t_player	*player;
	void		*img;

	player = world->player;
	world->player->mapped_pos = iso_map((t_xy){player->pos.x * grid.space_x,
			player->pos.y * grid.space_y});
	img = player->animator.frames[player->animator.current_frame];
	render_tile(display, grid, img, player->pos, (t_xy){12, -44});
	return (1);
}

int	render_floor(t_world *world, t_display *display, t_grid_d grid, t_data **sprites)
{
	t_xy	tile;
	t_data	*b_image;
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
			render_tile(display, grid, b_image, (t_xy){tile.x, tile.y}, 
				(t_xy){0, (tgrid[tile.y][tile.x].type == '1') * -20});
		}
	}
	return (1);
}

int	render_sentry(t_display *display, t_grid_d grid, t_enemy *enemy)
{
	enemy->current_pos = bounce((t_xy){enemy->pos.x * grid.space_x,
			enemy->pos.y * grid.space_y}, enemy->current_pos, 2);
	mlx_put_image_to_window(display->mlx, display->mlx_win,
		enemy->animator->frames[enemy->animator->current_frame]->img,
		iso_map(enemy->current_pos).x + grid.offset_x
		+ center(display->sprites[1], g_frame(enemy, 1)),
		iso_map(enemy->current_pos).y + grid.offset_y -32);
	return (1);
}

void	render_obj(t_world *world, t_display *display, t_grid_d grid, t_xy tile)
{
	t_tile		current_tile;
	t_data		**sprites;

	sprites = display->sprites;
	current_tile = world->tgrid[tile.y][tile.x];
	if (current_tile.type == '1')
		render_tile(display, grid, sprites[1], (t_xy){tile.x, tile.y},
			(t_xy){0, (current_tile.type == '1') * -30});
	if (current_tile.type == 'E')
		render_tile(display, grid, sprites[4],
			(t_xy){tile.x, tile.y}, (t_xy){0, -50});
	if (current_tile.type == 'S' && current_tile.data)
		render_sentry(display, grid, (t_enemy *)((current_tile.data)));
	if (current_tile.type == 'C' && current_tile.data)
		render_tile(display, grid, g_frame((t_coin *) \
	(current_tile.data), 0), tile, (t_xy){center(display->sprites[1], \
			g_frame((t_coin *)(current_tile.data), 0)), -20});
	if (tile.x == world->player->pos.x && tile.y == world->player->pos.y)
		render_player(world, grid, display);
}

void	render_world(t_world *world, t_display *display, t_grid_d grid)
{
	t_xy		tile;

	render_floor(world, display, grid, display->sprites);
	tile.y = -1;
	while (world->tgrid[++tile.y])
	{
		tile.x = -1;
		while (world->tgrid[tile.y][++tile.x].type)
			render_obj(world, display, grid, tile);
	}
}

int draw_fov(t_enemy *enemy, t_display *display,
	t_grid_d grid, t_data *char_array)
{
	t_xy	pos;

	draw_rect(char_array, (t_xy){100, 60}, (t_xy){0, 0}, 0xFFFF0000);
	pos = (t_xy){30, 0};
	draw_line(char_array, iso_map(pos),
		iso_map(moveInDirection(pos, enemy->c_ang + 20, 15)), 0x30eeff);
	draw_line(char_array, iso_map(pos),
		iso_map(moveInDirection(pos, enemy->c_ang -20, 15)), 0x30eeff);
	mlx_put_image_to_window(display->mlx, display->mlx_win, char_array->img,
		iso_map((t_xy){enemy->pos.x * 21, enemy->pos.y * 21}).x
		+ grid.offset_x - 14,
		iso_map((t_xy){enemy->pos.x * 21, enemy->pos.y * 21}).y
		+ grid.offset_y - 50);
	return (1);
}
