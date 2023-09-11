#include "graphics.h"

int	handle_keypress(int keycode, t_frame *frame)
{
	t_player	*player;
	t_xy		new_pos;
	t_tile		t;

	player = frame->world->player;
	new_pos = player->pos;
	if (keycode == 13)
		new_pos.y --;
	else if (keycode == 0)
		new_pos.x --;
	else if (keycode == 1)
		new_pos.y ++;
	else if (keycode == 2)
		new_pos.x ++;
	t = frame->world->tgrid[new_pos.y][new_pos.x];
	if (new_pos.x > 0 && new_pos.y > 0 && t.type != '1' && t.type != 'S')
	{
		player->pos = new_pos;
		frame->world->count.moves ++;
	}
	if (t.type == 'E')
		endgame(frame->world, frame->display);
	if ((frame->world->tgrid[player->pos.y][player->pos.x].type) == 'C')
		frame->world->tgrid[player->pos.y][player->pos.x].type = 'G';
	return (0);
}


int	render_next_frame(void *param)
{
	t_frame			*data;
	t_world			*world;
	t_display		*display;
	char			*buffer;

	data = (t_frame *)param;
	world = data->world;
	display = data->display;
	(*(data->frame_sec))++;
	if (*(data->frame_sec) < 100)
		return (1);
	mlx_clear_window(display->mlx, display->mlx_win);
	*(data->frame_sec) = 0;
	display->camera->pos = bounce(display->camera->pos,
			(t_xy){world->player->mapped_pos.x + (display->mouse.x
				- (display->dimensions.x / 2)) / 3, world->player->mapped_pos.y \
		+ (display->mouse.y - (display->dimensions.y / 2)) / 3}, 10);
	render_world(world, display, (t_grid_d){
		21, 21, ((display->width) / 2) - display->camera->pos.x,
		((display->height) / 2) - display->camera->pos.y});
	update_animations(display, world);
	buffer = ft_itoa(world->count.moves);
	mlx_string_put(display->mlx, display->mlx_win, 10, 10, 0x00FF0000, buffer);
	free(buffer);
	return (1);
}

int	mouse(int x, int y, void *param)
{
	t_frame	*char_array;

	char_array = (t_frame *)param;
	char_array->display->mouse.x = x;
	char_array->display->mouse.y = y;
	return (1);
}
