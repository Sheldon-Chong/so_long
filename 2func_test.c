#include <stdio.h>
#include "graphics.h"
#include <fcntl.h>

int handle_keypress(int keycode, t_frame *current_frame)
{
	t_player *player = current_frame->world->player;
	t_xy new_pos = player->pos;

	if (keycode == 13 && current_frame->world->tgrid[player->pos.y - 1][player->pos.x].type != '1') // 'W' key is pressed
		new_pos.y --;
	else if (keycode == 0 && current_frame->world->tgrid[player->pos.y][player->pos.x - 1].type != '1') // 'A' key is pressed
		new_pos.x --;
	else if (keycode == 1 && current_frame->world->tgrid[player->pos.y + 1][player->pos.x].type != '1') // 'S' key is pressed
		new_pos.y ++;
	else if (keycode == 2 && current_frame->world->tgrid[player->pos.y][player->pos.x + 1].type != '1') // 'D' key is pressed
		new_pos.x ++;
	if(new_pos.x > 0 && new_pos.y > 0)
	{
		player->pos = new_pos;
		current_frame->world->movement_count ++;
	}
	if(current_frame->world->tgrid[player->pos.y][player->pos.x].type == 'H')
	{
		
	}
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

int render_player(t_world *world, t_xy pos, t_display *display)
{
	void	*mlx	 = display->mlx;
	void	*mlx_win = display->mlx_win;
	t_player *player = world->player;
	char	**grid = world->grid;

	void	*img;
	
	img = player->animator.frames[player->animator.current_frame]->img;
	mlx_put_image_to_window(mlx, mlx_win, img, pos.x, pos.y);	
	return 1;
}

t_xy moveInDirection(t_xy currentPos, double direction, double distance) {
	t_xy newPos;
	
	double radianDirection = direction * M_PI / 180.0; // Convert degrees to radians
	newPos.x = currentPos.x + distance * cos(radianDirection);
	newPos.y = currentPos.y + distance * sin(radianDirection);
	
	return newPos;
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

int render_tile(t_display *display, t_grid_display grid, t_data *b_image, t_xy pos, t_xy modify)
{
	mlx_put_image_to_window(display->mlx, display->mlx_win, b_image->img, 
		iso_map((t_xy){pos.x * grid.space_x, pos.y * grid.space_y}).x + grid.offset_x + modify.x,
		iso_map((t_xy){pos.x * grid.space_x, pos.y * grid.space_y}).y + grid.offset_y + modify.y);
	return 1;
}

int update_enemy(t_world *world, t_display *display, t_grid_display grid)
{
	int i = 0;
	t_object *head = world->enemies;
	t_data *test = new_img(display->mlx, 100,60);
	while(head)
	{
		draw_square(test, 100,60, 0,0,0xFFFF0000);
		t_xy pos = (t_xy){30, 0};
		t_xy enemy_pos = ((t_enemy *)(head->data))->pos;
		t_enemy	*enemy = ((t_enemy *)(head->data));
		enemy->angle = enemy->angle + (enemy->i_angle - enemy->angle) / 10;
		draw_line(test, iso_map(pos), iso_map(moveInDirection(pos, ((t_enemy *)(head->data))->angle + 20, 15)), 0x00FF0000);
		draw_line(test, iso_map(pos), iso_map(moveInDirection(pos, ((t_enemy *)(head->data))->angle -20, 15)), 0x00FF0000);
		mlx_put_image_to_window(display->mlx, display->mlx_win, test->img, 
			iso_map((t_xy){enemy_pos.x * 21, enemy_pos.y * 21}).x + grid.offset_x - 18,
			iso_map((t_xy){enemy_pos.x * 21, enemy_pos.y * 21}).y + grid.offset_y - 50);

		enemy->player_found = 0;
		if(ray_cast(world, enemy->pos, enemy->i_angle, 6, world->dimensions.y, world->dimensions.x) == 1 ||
			ray_cast(world, enemy->pos, enemy->i_angle + 20, 6, world->dimensions.y, world->dimensions.x) == 1 ||
			ray_cast(world, enemy->pos, enemy->i_angle - 20, 6, world->dimensions.y, world->dimensions.x) == 1
			)
			enemy->player_found = 1;				 	
		if(enemy->player_found == 1)
			render_tile(display, grid, display->sprites[1], enemy->pos, (t_xy){((display->sprites[1]->line_length)/4 - (enemy->animator->frames[0]->line_length)/4) / 2, - 40});
		move_enemy(enemy, (t_xy){enemy->pos.x + 1,4}, world);
		head = head->next;
	}
	free(test);
	return 1;
}

int render_world(t_world *world, t_display *display, t_grid_display grid, t_data **sprites)
{
	int		tile_x = -1;
	int		tile_y = -1;
	t_data	*b_image;
	t_xy 	b_xy;
	t_tile	**tgrid = world->tgrid;
	t_enemy	*enemy; 

	while(tgrid[++tile_y])
	{
		tile_x = -1;
		while(tgrid[tile_y][++tile_x].type)
		{	
			b_image = sprites[2];
			if(tile_x % 2 == 0 && tile_y % 2 != 0 || tile_x % 2 != 0 && tile_y % 2 == 0 )
				b_image = sprites[3];
			if(tgrid[tile_y][tile_x].type == '1')
				b_image = sprites[1];
			if(tgrid[tile_y][tile_x].type != 'H')
				render_tile(display, grid, b_image, (t_xy){tile_x, tile_y}, (t_xy){0, (tgrid[tile_y][tile_x].type == '1') * -20});
			if(tgrid[tile_y][tile_x].type == 'S' && tgrid[tile_y][tile_x].data)
			{
				enemy = (t_enemy *)((tgrid[tile_y][tile_x].data));
				render_tile(display, grid, enemy->animator->frames[enemy->animator->current_frame], 
				(t_xy){tile_x, tile_y}, (t_xy){((display->sprites[1]->line_length)/4 - (b_image->line_length)/4) / 2, -25});
			}
			if(tgrid[tile_y][tile_x].type == 'C' && tgrid[tile_y][tile_x].data)
			{
				t_collectible *collectible = ((t_collectible *)((t_object *)(tgrid[tile_y][tile_x].data))->data);
				render_tile(display, grid, collectible->animator->frames[collectible->animator->current_frame], (t_xy){tile_x, tile_y}, (t_xy){((display->sprites[1]->line_length)/4 - (b_image->line_length)/4) / 2, -20});
			}
			if(tile_x == world->player->pos.x && tile_y == world->player->pos.y)
			{
				world->player->mapped_pos = iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y});
				render_player(world, (t_xy){world->player->mapped_pos.x + grid.offset_x + 12, world->player->mapped_pos.y + grid.offset_y - 45}, display);
			}
		}
	}
	update_enemy(world, display, grid);
	return (1);
}

int render_next_frame(void *param)
{
	t_frame_data *data = (t_frame_data *)param;
	void *mlx = data->graphic_display->mlx;
	void *mlx_win = data->graphic_display->mlx_win;
	t_world *world = data->world;
	t_display *display = data->graphic_display;
	int mouse_x, mouse_y;
	long i = *(data->i);
	
	(*(data->frame_sec))++;
	if (*(data->frame_sec) > 100)
	{
		mlx_clear_window(mlx, mlx_win);
		*(data->frame_sec) = 0;
		display->camera->pos = bounce(display->camera->pos, world->player->mapped_pos);
		render_world(world, display, (t_grid_display){21, 21, ((display->width) / 2) - display->camera->pos.x, ((display->height) / 2) - display->camera->pos.y}, data->graphic_display->sprites);
		
		t_object	*head = display->animations;
		t_animation	*animator;
		while(head)
		{
			animator = ((t_animation *)(head->data));
			animator->frame_timer = (animator->frame_timer + 1) % animator->speed;
			if(animator->frame_timer == 0)
				animator->current_frame = (animator->current_frame + 1) % 2;
			head = head->next;
		}
		mlx_string_put(mlx,mlx_win, 10, 10, 0x00FF0000, ft_itoa(world->movement_count));
		(*(data->i))++;
		head = world->enemies;
		while(head)
		{
			if (generateRandomInt(1, 100) == 1)
			 	((t_enemy *)(head->data))->i_angle = generateRandomInt(1, 360);
			head = head->next;
		}
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


void get_objects(char **array, t_animation *animator, t_world *world, t_display *display)
{
	int 		x;
	int 		y;

	t_counter *count = malloc(sizeof(t_counter));
	*count = (t_counter){0,0,0,0,0,0,0};

	t_object	*sentry_list = NULL;
	t_object	*collectible_list = NULL;
	y = -1;
	while(array[++y])
	{
		x = -1;
		while(array[y][++x])
		{
			if (array[y][x] == 'C')
			{
				count->collectible ++;
				t_collectible *collectible = malloc(sizeof(t_collectible));
				*collectible = (t_collectible){(t_xy){x, y}, (t_animation *)(display->anim_spritesheet->data)};
				object_add_back(&collectible_list, new_object("collectible", collectible));
			}
			if (array[y][x] == '0') 
				count->empty ++;
			if (array[y][x] == '1') 
				count->wall ++;
			if (array[y][x] == 'E') 
				count->exit ++;
			if (array[y][x] == 'S')
			{
				count->sentry ++;
				t_enemy *sentry = malloc(sizeof(t_enemy));
				*sentry = (t_enemy){(t_xy){x,y}, 0, 0, 5, animator, 0};
				object_add_back(&sentry_list, new_object("enemy", sentry));
			}
			if (array[y][x] == 'P')
			{
				world->player->pos = (t_xy){x,y};
				count->player ++;
			}
			if (array[y][x] == 'H')
				count->hole ++;
		}
	}
	if(count->player > 1)
		exit(write(1, "More than 1 player\n", 19));
	printf("player: %d\n",count->player);
	printf("sentry: %d\n",count->sentry);
	printf("coin: %d\n",count->collectible);
	printf("wall: %d\n",count->wall);
	printf("empty: %d\n",count->empty);
	printf("hole: %d\n",count->hole);
	printf("exit: %d\n",count->exit);

	world->enemies = sentry_list;
	world->collectibles = collectible_list;
}

t_data	**frames(char *frames, t_display *display)
{
	t_data **array;
	char	**char_array = ft_split(frames, ',');
	int i = 0;
	while(char_array[i])
		i ++;
	array = malloc(sizeof(t_data *) * (i + 1));
	array[i - 1] = NULL;
	i = -1;
	while(char_array[++i])
		array[i] = put_img(char_array[i], display->mlx);

	return(array);
}

int	main(void)
{
	t_display			*display = graphics_init(1920, 1080);
	t_player 			*player = malloc(sizeof(t_player));
	t_world 			*world = malloc(sizeof(t_world));

	t_animation	*enemy_animation = malloc(sizeof(t_animation));
	*enemy_animation = (t_animation){0, 0, 5, frames("assets/sentry.xpm,assets/player.xpm", display)};

	*player = (t_player){(t_xy){1, 1},(t_xy){1, 1},(t_xy){1, 1},4,0};
	player->animator.current_frame = 0;
	player->animator.frames = frames("assets/player.xpm,assets/player.xpm", display);
	player->animator.speed = 30;

	t_animation	*coin_animation = malloc(sizeof(t_animation));
	*coin_animation = (t_animation){0, 0, 5};
	coin_animation->frames = frames("assets/collectible.xpm,assets/collectible.xpm", display);
	coin_animation->speed = 30;
	
	char	**test = scan_map(world);
	
	world = &((t_world){player, 0, NULL, NULL, test, NULL, 0, world->dimensions});

	object_add_back(&display->anim_spritesheet, new_object("animation", coin_animation));

	printf("<%d>\n", ((t_animation *)(display->anim_spritesheet->data))->current_frame);
	get_objects(test, enemy_animation, world, display);
	t_object	*head = world->collectibles;
	while(head)
	{
		printf("%d]]]\n", ((t_collectible *)(head->data))->pos.x);
		head = head->next;
	}

	printf("\nENEMIES:\n");
	head = world->enemies;
	while(head)
	{
		printf("Position: (%d, %d)\n", ((t_enemy *)(head->data))->pos.x, ((t_enemy *)(head->data))->pos.y);
		printf("HP: (%d)\n", ((t_enemy *)(head->data))->hp);
		head = head->next;
	}

	object_add_back(&display->animations, new_object("animation",  enemy_animation));
	object_add_back(&display->animations, new_object("animation",  coin_animation));
	object_add_back(&display->animations, new_object("player animation",  &player->animator));
	
	world->tgrid = read_array2(world, count_newline("test.ber"));
	print_2d_tiles(world->tgrid);

	t_frame vars;
	vars.world = world;
	vars.display = display;

	int		sec = 0;
	int		frame_sec = 0;
	int		i;
	t_frame_data current_frame = (t_frame_data){&frame_sec, world, display, &i};
	
	mlx_hook(display->mlx_win, 2, 1L << 0, handle_keypress, &vars);
	mlx_loop_hook(display->mlx, render_next_frame, &current_frame);
	mlx_loop(display->mlx);
}

