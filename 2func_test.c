#include <stdio.h>
#include "graphics.h"
#include <fcntl.h>

int endgame(t_world *world, t_display *display)
{
	mlx_destroy_window(display->mlx, display->mlx_win);
	exit(0);
}

int handle_keypress(int keycode, t_frame_data *current_frame)
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
		current_frame->world->movement_count ++;
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

int draw_fov(t_enemy *enemy, t_world *world, t_display *display, t_grid_display grid, t_data *char_array)
{
	draw_square(char_array, 100,60, 0,0,0xFFFF0000);
	t_xy pos = (t_xy){30, 0};
	draw_line(char_array, iso_map(pos), iso_map(moveInDirection(pos, enemy->current_angle + 20, 15)), 0x30eeff);
	draw_line(char_array, iso_map(pos), iso_map(moveInDirection(pos, enemy->current_angle -20, 15)), 0x30eeff);
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

int enemy_search4player(t_world *world, t_display *display, t_enemy *enemy)
{
	if(enemy->alert > 0)
	{
		enemy->final_angle = pts2angle(enemy->pos, world->player->pos);
		enemy->current_angle = pts2angle(enemy->pos, world->player->pos);
	}
	if(ray_cast(world, enemy->pos, enemy->current_angle, 10, world->dimensions.y, world->dimensions.x) == 1 ||
		ray_cast(world, enemy->pos, enemy->current_angle + 20, 10, world->dimensions.y, world->dimensions.x) == 1 ||
		ray_cast(world, enemy->pos, enemy->current_angle - 20, 10, world->dimensions.y, world->dimensions.x) == 1)
	{
		enemy->player_found = 1;
		enemy->final_angle = enemy->current_angle;
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
		enemy->current_angle = enemy->current_angle + (enemy->final_angle - enemy->current_angle) / 10;
		draw_fov(enemy, world, display, grid, char_array);
		enemy->player_found = 0;
		enemy_search4player(world, display, enemy);
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
			if(tile.x % 2 == 0 && tile.y % 2 != 0 || tile.x % 2 != 0 && tile.y % 2 == 0 )
				b_image = sprites[3];
			if(tgrid[tile.y][tile.x].type != 'H')
				render_tile(display, grid, b_image, (t_xy){tile.x, tile.y}, (t_xy){0, (tgrid[tile.y][tile.x].type == '1') * -20});	
		}
	}
	return (1);
}

int render_enemy(t_world *world, t_display *display, t_grid_display grid, t_enemy *enemy)
{
	enemy->current_pos = bounce((t_xy){enemy->pos.x * grid.space_x, enemy->pos.y * grid.space_y}, enemy->current_pos, 2);
	mlx_put_image_to_window(display->mlx, display->mlx_win, enemy->animator->frames[enemy->animator->current_frame]->img, 
		iso_map(enemy->current_pos).x + grid.offset_x + center(display->sprites[1], g_frame(enemy, 1)),
		iso_map(enemy->current_pos).y + grid.offset_y -32);
	return (1);
}

int render_object(t_world *world, t_display *display, t_grid_display grid, void *object)
{
	t_collectible *ret = (t_collectible *)object;
	mlx_put_image_to_window(display->mlx, display->mlx_win, ret->animator->frames[ret->animator->current_frame]->img, 
		iso_map(ret->pos).x + grid.offset_x + ((display->sprites[1]->line_length)/4 - (g_frame(ret, 0)->line_length)/4) / 2, 
		iso_map(ret->pos).y + grid.offset_y -32);
	return (1);
}

int render_world(t_world *world, t_display *display, t_grid_display grid, t_data **sprites)
{
	t_xy		tile;
	t_data		*b_image;
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
				render_enemy(world,display,grid,(t_enemy *)((current_tile.data)));
			if(current_tile.type == 'C' && current_tile.data)
				render_tile(display, grid, g_frame((t_collectible *)(current_tile.data), 0), tile, (t_xy)
								{center(display->sprites[1], g_frame((t_collectible *)(current_tile.data), 0)), -20});
			if(tile.x == world->player->pos.x && tile.y == world->player->pos.y)
				render_player(world, grid, display);
		}
	}
	return (1);
}

int update_animations(t_display *display, t_world *world)
{
	t_object	*head = display->animations;
	t_animation	*animator;
	while(head)
	{
		animator = ((t_animation *)(head->data));
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
	t_frame_data	*data;
	t_world			*world;
	t_display		*display;
	
	data =(t_frame_data *)param;
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
		mlx_string_put(display->mlx,display->mlx_win, 10, 10, 0x00FF0000, ft_itoa(world->movement_count));
	}
	return (1);
}

typedef struct t_counter{
	int player;
	int sentry;
	int collectible;
	int wall;
	int empty;
	int hole;
	int exit;
} t_counter;

void new_sentry(t_object *sentry_list, t_object *timer_list, t_xy pos, t_animation	*animator)
{
	t_enemy *sentry = malloc(sizeof(t_enemy));
	*sentry = (t_enemy){pos, 0, 0, 5, animator, 0, 0, (t_xy){0,0}};
	object_add_back(&sentry_list, new_object("enemy", sentry));

	t_timer *timer = malloc(sizeof(t_collectible));
	*timer = (t_timer){5,0};
	object_add_back(&(sentry->timers), new_object("timer", timer));
}

void get_objects(char **array, t_animation *animator, t_world *world, t_display *display)
{
	int 		x;
	int 		y;
	t_counter	count;
	
	count = (t_counter){0,0,0,0,0,0,0};

	t_object	*sentry_list = NULL;
	t_object	*collectible_list = NULL;
	t_object	*timer_list = NULL;
	y = -1;
	while(array[++y])
	{
		x = -1;
		while(array[y][++x])
		{
			if (array[y][x] == 'C')
			{
				count.collectible ++;
				t_collectible *collectible = malloc(sizeof(t_collectible));
				*collectible = (t_collectible){(t_xy){x, y}, (t_animation *)(display->anim_spritesheet->data)};
				object_add_back(&collectible_list, new_object("collectible", collectible));
			}
			if (array[y][x] == 'E') 
				count.exit ++;
			if (array[y][x] == 'S')
			{
				t_enemy *sentry = malloc(sizeof(t_enemy));
				*sentry = (t_enemy){(t_xy){x,y}, 0, 0, 5, animator, 0, 0, (t_xy){0,0}};
				object_add_back(&sentry_list, new_object("enemy", sentry));

				t_timer *timer = malloc(sizeof(t_collectible));
				*timer = (t_timer){5,0};
				object_add_back(&(sentry->timers), new_object("timer", timer));
			}
			if (array[y][x] == 'P')
			{
				world->player->pos = (t_xy){x,y};
				count.player ++;
			}
		}
	}
	if(count.player > 1 )
		exit(write(1, "Error: More than 1 player\n", 26));
	printf("player: %d\n",count.player);
	printf("sentry: %d\n",count.sentry);
	printf("coin: %d\n",count.collectible);
	printf("exit: %d\n",count.exit);

	world->enemies = sentry_list;
	world->collectibles = collectible_list;
}

t_data	**frames(char *frames, t_display *display)
{
	t_data	**array;
	char	**char_array;
	int		i;

	char_array = ft_split(frames, ',');
	i = 0;
	while(char_array[i])
		i ++;
	array = malloc(sizeof(t_data *) * (i + 1));
	array[i - 1] = NULL;
	i = -1;
	while(char_array[++i])
		array[i] = put_img(char_array[i], display->mlx);
	return(array);
}


int mouse(int x, int y, void *param)
{
	t_frame_data *char_array = (t_frame_data *)param;
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

int	exit_accessible(char **c, t_world *world)
{
	int exit_found;
	
	exit_found = 0;
	recur(c, world->player->pos, world, &exit_found);
	return (exit_found);
}

void	print_statistics(t_world *world)
{
	t_object *head;

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
		printf("Position: (%d, %d)\n", ((t_collectible *)(head->data))->pos.x, ((t_collectible *)(head->data))->pos.y);
		head = head->next;
	}
}

int	main(void)
{
	t_display			*display = display_init(1920, 1080);
	t_player 			*player = malloc(sizeof(t_player));
	t_world 			*world = malloc(sizeof(t_world));

	t_animation	*enemy_animation;
	enemy_animation = &(t_animation){0, 0, 5, frames("assets/sentry.xpm,assets/sentry.xpm", display)};

	*player = (t_player){(t_xy){1, 1},(t_xy){1, 1},(t_xy){1, 1}, 4, 0};
	player->animator = (t_animation){0, 0, 30, frames("assets/player.xpm,assets/player.xpm", display)};

	t_animation	*coin_animation;
	coin_animation = &(t_animation){0, 0, 30, frames("assets/collectible.xpm,assets/player.xpm", display)};
	
	char	**char_array = scan_map(world);
	world = &((t_world){player, 0, NULL, NULL, char_array, NULL, 0, world->dimensions});

	object_add_back(&display->anim_spritesheet, new_object("animation", coin_animation));

	get_objects(char_array, enemy_animation, world, display);
	t_object	*head = world->collectibles;

	print_statistics(world);

	object_add_back(&display->animations, new_object("animation",  enemy_animation));
	object_add_back(&display->animations, new_object("animation",  coin_animation));
	object_add_back(&display->animations, new_object("player animation",  &player->animator));
	
	world->tgrid = read_array2(world, count_newline("char_array.ber"));
	print_2d_tiles(world->tgrid);

	if(exit_accessible(char_array, world) == 0)
		exit(write(2, "Error: Player cannot reach exit\n", 32));

	int		frame_sec = 0;
	int		i;
	t_frame_data current_frame = (t_frame_data){&frame_sec, world, display, &i};

	mlx_hook(display->mlx_win, 2, 1L << 0, handle_keypress, &current_frame);
	mlx_loop_hook(display->mlx, render_next_frame, &current_frame);
	mlx_hook(display->mlx_win, 6, 1L << 6, mouse, &current_frame);    

	mlx_loop(display->mlx);
}

