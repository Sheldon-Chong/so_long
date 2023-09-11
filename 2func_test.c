#include <stdio.h>
#include "graphics.h"
#include <fcntl.h>

int endgame(t_world *world, t_display *display)
{
	mlx_destroy_window(display->mlx, display->mlx_win);
	system("leaks program");

	exit(0);
}

int handle_keypress(int keycode, t_frame *current_frame)
{
	t_player *player = current_frame->world->player;
	t_xy new_pos = player->pos;
	t_tile tile;

	if (keycode == 13)     //'W' key is pressed
		new_pos.y --;
	else if (keycode == 0) // 'A' key is pressed
		new_pos.x --;
	else if (keycode == 1) // 'S' key is pressed
		new_pos.y ++;
	else if (keycode == 2) // 'D' key is pressed
		new_pos.x ++;
	tile = current_frame->world->tgrid[new_pos.y][new_pos.x];
	if(new_pos.x > 0 && new_pos.y > 0 && tile.type != '1' && tile.type != 'S')
	{
		player->pos = new_pos;
		current_frame->world->count.moves ++;
	}
	if (tile.type == 'E')
		endgame(current_frame->world, current_frame->display);
	if((current_frame->world->tgrid[player->pos.y][player->pos.x].type) == 'C')
		current_frame->world->tgrid[player->pos.y][player->pos.x].type = 'G';
	return (0);
}

typedef struct s_grid_display {
	float		space_x;
	float		space_y;
	int			offset_x;
	int			offset_y;
} t_grid_display;

typedef struct s_grid {
	int width;
	int height;
	char **grid;
} t_grid;


t_xy moveInDirection(t_xy currentPos, double direction, double distance) {
	t_xy newPos;
	
	double radianDirection = direction * M_PI / 180.0; // Convert degrees to radians
	newPos.x = currentPos.x + distance * cos(radianDirection);
	newPos.y = currentPos.y + distance * sin(radianDirection);
	
	return (newPos);
}

void	move_enemy(t_enemy *enemy, t_xy pos, t_world *world)
{
	if(pos.x < world->dimensions.x && pos.y < world->dimensions.y && 
		world->tgrid[pos.y][pos.x].type != 'S' && world->tgrid[pos.y][pos.x].type != '1' )
	{
		world->tgrid[enemy->pos.y][enemy->pos.x].type = '0';
		world->tgrid[enemy->pos.y][enemy->pos.x].data = NULL;
		enemy->pos = pos;
		world->tgrid[pos.y][pos.x].type = 'S';
		world->tgrid[pos.y][pos.x].data = enemy;
	}	
}

int draw_fov(t_enemy *enemy, t_display *display, t_grid_display grid, t_data *char_array)
{
	draw_square(char_array, 100,60, 0,0,0xFFFF0000);
	t_xy pos = (t_xy){30, 0};
	draw_line(char_array, iso_map(pos), iso_map(moveInDirection(pos, enemy->c_ang + 20, 15)), 0x30eeff);
	draw_line(char_array, iso_map(pos), iso_map(moveInDirection(pos, enemy->c_ang -20, 15)), 0x30eeff);
		mlx_put_image_to_window(display->mlx, display->mlx_win, char_array->img, 
			iso_map((t_xy){enemy->pos.x * 21, enemy->pos.y * 21}).x + grid.offset_x - 14,
			iso_map((t_xy){enemy->pos.x * 21, enemy->pos.y * 21}).y + grid.offset_y - 50);
	return (1);
}

t_xy render_tile(t_display *display, t_grid_display grid, t_data *b_image, t_xy pos, t_xy modify)
{
	t_xy ret = (t_xy){
		iso_map((t_xy){pos.x * grid.space_x, pos.y * grid.space_y}).x + grid.offset_x + modify.x,
		iso_map((t_xy){pos.x * grid.space_x, pos.y * grid.space_y}).y + grid.offset_y + modify.y};

	if(b_image == NULL)
		return (ret);
	mlx_put_image_to_window(display->mlx, display->mlx_win, b_image->img, ret.x, ret.y);
		
	return (ret);
}

int render_player(t_world *world, t_grid_display grid, t_display *display)
{
	t_player	*player;
	void		*img;
	
	player = world->player;
	world->player->mapped_pos = iso_map((t_xy){player->pos.x * grid.space_x, player->pos.y * grid.space_y});
	img = player->animator.frames[player->animator.current_frame];
	render_tile(display, grid, img, player->pos, (t_xy){12, -44});
	return (1);
}

int decide_enemy_movement(t_world *world, t_enemy *enemy)
{
	if(world->player->pos.x - enemy->pos.x > 0)
		move_enemy(enemy, (t_xy){enemy->pos.x + 1, enemy->pos.y}, world);
	if(world->player->pos.x - enemy->pos.x < 0)
		move_enemy(enemy, (t_xy){enemy->pos.x - 1, enemy->pos.y}, world);
	if(world->player->pos.y - enemy->pos.y > 0)
		move_enemy(enemy, (t_xy){enemy->pos.x, enemy->pos.y + 1}, world);
	if(world->player->pos.y - enemy->pos.y < 0)
		move_enemy(enemy, (t_xy){enemy->pos.x, enemy->pos.y - 1}, world);
	return (1);
}

int enemy_search4player(t_world *world, t_enemy *enemy)
{
	if(enemy->alert > 0)
	{
		enemy->final_angle = pts2angle(enemy->pos, world->player->pos);
		enemy->c_ang = pts2angle(enemy->pos, world->player->pos);
	}
	if(ray_cast(world, enemy->pos, enemy->c_ang, 10, world->dimensions.y, world->dimensions.x) == 1 ||
		ray_cast(world, enemy->pos, enemy->c_ang + 20, 10, world->dimensions.y, world->dimensions.x) == 1 ||
		ray_cast(world, enemy->pos, enemy->c_ang - 20, 10, world->dimensions.y, world->dimensions.x) == 1)
	{
		enemy->player_found = 1;
		enemy->final_angle = enemy->c_ang;
		enemy->alert = 100;
	}
	else
		enemy->alert = 0;
	return (1);
}

int update_enemy(t_world *world, t_display *display, t_grid_display grid)
{
	t_enemy		*enemy;
	t_object	*head;
	t_data		*char_array;
	
	char_array = new_img(display->mlx, 100,60);
	head = world->enemies;
	while(head)
	{
		enemy = (t_enemy *)(head->data);
		if(enemy->pos.x == world->player->pos.x && enemy->pos.y == world->player->pos.y)
			endgame(world, display);
		enemy->c_ang = enemy->c_ang + (enemy->final_angle - enemy->c_ang) / 10;
		draw_fov(enemy, display, grid, char_array);
		enemy->player_found = 0;
		enemy_search4player(world, enemy);
		if (generateRandomInt(1, 100) == 1 && enemy->player_found == 0)
			enemy->final_angle += generateRandomInt(-180, 180);
		if (enemy->player_found == 1 && (((t_timer *)(enemy->timers->data))->elapsed == 0))
			decide_enemy_movement(world, enemy);
		enemy->alert -= (enemy->alert > 0);
		head = head->next;
		((t_timer *)(enemy->timers->data))->elapsed ++;
		if(((t_timer *)(enemy->timers->data))->elapsed > 70)
			((t_timer *)(enemy->timers->data))->elapsed = 0;
	}
	free(char_array);
	return 1;
}

int render_floor(t_world *world, t_display *display, t_grid_display grid, t_data **sprites)
{
	t_xy tile;
	t_data	*b_image;
	t_tile	**tgrid;

	tgrid = world->tgrid;
	tile.y = -1;
	while(tgrid[++tile.y])
	{
		tile.x = -1;
		while(tgrid[tile.y][++tile.x].type)
		{	
			b_image = sprites[2];
			if((tile.x % 2 == 0 && tile.y % 2 != 0 )|| (tile.x % 2 != 0 && tile.y % 2 == 0) )
				b_image = sprites[3];
			render_tile(display, grid, b_image, (t_xy){tile.x, tile.y}, 
				(t_xy){0, (tgrid[tile.y][tile.x].type == '1') * -20});	
		}
	}
	return (1);
}

int render_enemy(t_display *display, t_grid_display grid, t_enemy *enemy)
{
	enemy->current_pos = bounce((t_xy){enemy->pos.x * grid.space_x, enemy->pos.y * grid.space_y}, enemy->current_pos, 2);
	mlx_put_image_to_window(display->mlx, display->mlx_win, enemy->animator->frames[enemy->animator->current_frame]->img, 
		iso_map(enemy->current_pos).x + grid.offset_x + center(display->sprites[1], g_frame(enemy, 1)),
		iso_map(enemy->current_pos).y + grid.offset_y -32);
	return (1);
}

// int render_object(t_display *display, t_grid_display grid, void *object)
// {
// 	t_coin *ret = (t_coin *)object;
// 	mlx_put_image_to_window(display->mlx, display->mlx_win, ret->animator->frames[ret->animator->current_frame]->img, 
// 		iso_map(ret->pos).x + grid.offset_x + ((display->sprites[1]->line_length)/4 - (g_frame(ret, 0)->line_length)/4) / 2, 
// 		iso_map(ret->pos).y + grid.offset_y -32);
// 	return (1);
// }

int render_world(t_world *world, t_display *display, t_grid_display grid, t_data **sprites)
{
	t_xy		tile;
	t_tile		current_tile;

	render_floor(world, display, grid, sprites);
	tile.y = -1;
	while(world->tgrid[++tile.y])
	{
		tile.x = -1;
		while(world->tgrid[tile.y][++tile.x].type)
		{
			current_tile = world->tgrid[tile.y][tile.x];
			if(current_tile.type == '1')
				render_tile(display, grid, sprites[1], (t_xy){tile.x, tile.y}, (t_xy){0, (current_tile.type == '1') * -30});	
			if(current_tile.type == 'E')
				render_tile(display, grid, sprites[4], (t_xy){tile.x, tile.y}, (t_xy){0, -50});	
			if(current_tile.type == 'S' && current_tile.data)
				render_enemy(display,grid,(t_enemy *)((current_tile.data)));
			if(current_tile.type == 'C' && current_tile.data)
				render_tile(display, grid, g_frame((t_coin *)(current_tile.data), 0), tile, (t_xy)
								{center(display->sprites[1], g_frame((t_coin *)(current_tile.data), 0)), -20});
			if(tile.x == world->player->pos.x && tile.y == world->player->pos.y)
				render_player(world, grid, display);
		}
	}
	return (1);
}

int update_animations(t_display *display, t_world *world)
{
	t_object	*head = display->animations;
	t_animator	*animator;
	while(head)
	{
		animator = ((t_animator *)(head->data));
		animator->frame_timer = (animator->frame_timer + 1) % animator->speed;
		if(animator->frame_timer == 0)
			animator->current_frame = (animator->current_frame + 1) % 2;
		update_enemy(world, display, (t_grid_display){21, 21, ((display->width) / 2) - display->camera->pos.x, ((display->height) / 2) - display->camera->pos.y});
		head = head->next;
	}
	return (1);
}

int render_next_frame(void *param)
{
	t_frame			*data;
	t_world			*world;
	t_display		*display;
	
	data =(t_frame *)param;
	world = data->world;
	display = data->display;
	(*(data->frame_sec))++;
	if (*(data->frame_sec) > 100)
	{
		mlx_clear_window(display->mlx, display->mlx_win);
		*(data->frame_sec) = 0;
		display->camera->pos = bounce(display->camera->pos, 
			(t_xy){world->player->mapped_pos.x + (display->mouse_pos.x - (display->dimensions.x / 2)) / 3,
			world->player->mapped_pos.y + (display->mouse_pos.y - (display->dimensions.y / 2)) / 3}, 10);
		render_world(world, display, (t_grid_display){
				21, 21, ((display->width) / 2) - display->camera->pos.x, 
				((display->height) / 2) - display->camera->pos.y}, 
		data->display->sprites);
		update_animations(display, world);
		mlx_string_put(display->mlx,display->mlx_win, 10, 10, 0x00FF0000, ft_itoa(world->count.moves));
	}
	return (1);
}

void new_sentry(t_object *sentry_list, t_xy pos, t_animator	*animator)
{
	t_enemy *sentry = malloc(sizeof(t_enemy));
	*sentry = (t_enemy){pos, 0, 0, 5, animator, 0, 0, (t_xy){0,0}, 0};
	append(&sentry_list, new_obj("enemy", sentry));

	t_timer *timer = malloc(sizeof(t_coin));
	*timer = (t_timer){5,0};
	append(&(sentry->timers), new_obj("timer", timer));
}

t_data	**frames(char *frames, char *directory, t_display *display)
{
	t_data	**array;
	char	**char_array;
	char	*path;
	int		i;

	char_array = ft_split(frames, ',');
	i = 0;
	while(char_array[i])
		i ++;
	array = malloc(sizeof(t_data *) * (i + 1));
	array[i - 1] = NULL;
	i = -1;
	while(char_array[++i])
	{
		path = ft_strjoin(directory, char_array[i]);
		array[i] = put_img(path, display->mlx);
	}
	return(array);
}


int mouse(int x, int y, void *param)
{
	t_frame *char_array = (t_frame *)param;
	char_array->display->mouse_pos.x = x;
	char_array->display->mouse_pos.y = y;
	return (1);
}

void recur(char **c, t_xy pos, t_world *world, int *exit_found)
{
	if(!(pos.x < world->dimensions.x && pos.y < world->dimensions.y && pos.x >= 0 && pos.y >= 0 && c[pos.y][pos.x] != '1' && c[pos.y][pos.x] != 'L'))
		return ;
	if(c[pos.y][pos.x] == 'E')
		*exit_found = 1;
	c[pos.y][pos.x] = 'L';
	recur(c, (t_xy){pos.x, pos.y + 1}, world, exit_found);
	recur(c, (t_xy){pos.x, pos.y - 1}, world, exit_found);
	recur(c, (t_xy){pos.x + 1, pos.y}, world, exit_found);
	recur(c, (t_xy){pos.x - 1, pos.y}, world, exit_found);
}



char **clone_char_array(char **c)
{
	int y = 0;
	char **ret_array;

	while(c[y])
		y ++;
	printf("LENGtH: %d\n", y);
	ret_array = malloc(sizeof(char *) * (y + 1));
	y = -1;
	while(c[++y])
		ret_array[y] = ft_strdup(c[y]);
	ret_array[y] = NULL;
	return(ret_array);
}

int	find_exit(char **c, t_world *world)
{
	int		exit_found;
	char	**dup;

	exit_found = 0;
	dup = clone_char_array(c);
	recur(dup, world->player->pos, world, &exit_found);
	ft_free_array((void **)dup, 0);
	return (exit_found);
}

void	print_statistics(t_world *world)
{
	t_object	*head;

	printf("\nENEMIES:\n");
	head = world->enemies;
	while(head)
	{
		printf("Position: (%d, %d)\n", ((t_enemy *)(head->data))->pos.x, ((t_enemy *)(head->data))->pos.y);
		printf("HP: (%d)\n", ((t_enemy *)(head->data))->hp);
		head = head->next;
	}
	printf("\nCOLLECtIBLES:\n");
	head = world->enemies;
	while(head)
	{
		printf("Position: (%d, %d)\n", ((t_coin *)(head->data))->pos.x, ((t_coin *)(head->data))->pos.y);
		head = head->next;
	}
}

void animation_init(t_world *world, t_display *display)
{
	t_animator	*coin_animation = malloc(sizeof(t_animator));
	*coin_animation = (t_animator){0, 0, 30, frames("collectible.xpm,player.xpm", "assets/", display)};

	t_animator	*enemy_animation = malloc(sizeof(t_animator));
	*enemy_animation = (t_animator){0, 0, 5, frames("sentry.xpm,sentry.xpm", "assets/", display)};

	world->player->animator = (t_animator){0, 0, 30, frames("player.xpm,player.xpm", "assets/", display)};

	append(&display->anim_spritesheet, new_obj("animation", coin_animation));
	append(&display->animations, new_obj("animation",  coin_animation));
	append(&display->anim_spritesheet, new_obj("animation", enemy_animation));
	append(&display->animations, new_obj("animation",  enemy_animation));
	append(&display->animations, new_obj("player animation",  &(world->player->animator)));
}

t_world *world_init(char *map)
{
	t_world		*world;
	t_player	*player;

	player = malloc(sizeof(t_player));
	world = malloc(sizeof(t_world));
	world->count=(t_counter){0,0,0,0,0};
	*world = ((t_world){player, NULL, NULL, scan_map(world, map), NULL, world->dimensions, 0});
	*player = (t_player){(t_xy){1, 1},(t_xy){1, 1}, (t_animator){0,0,0,0}};
	return(world);
}

int	main(void)
{
	t_display	*display;
	t_world		*world;
	t_frame		frame;

	display = display_init(1920, 1080);
	world = world_init("map.ber");
	printf("%d,%d\n",world->player->pos.x, world->player->pos.y);
	animation_init(world, display);
	//get_objects(char_array, world, display);
	world->coins = NULL;
	world->enemies = NULL;
	world->tgrid = char2tile(world, count_newline("map.ber"), display);
	if(find_exit(world->grid, world) == 0)
		exit(write(2, "Error: Player cannot reach exit\n", 32));

	print_statistics(world);
	//print_2d_tiles(world->tgrid);

	int		frame_sec = 0;
	frame = (t_frame){&frame_sec, world, display};
	mlx_hook(display->mlx_win, 2, 1L << 0, handle_keypress, &frame);
	mlx_loop_hook(display->mlx, render_next_frame, &frame);
	mlx_hook(display->mlx_win, 6, 1L << 6, mouse, &frame);

    

	mlx_loop(display->mlx);	
}
