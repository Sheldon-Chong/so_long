#include <stdio.h>
#include "graphics.h"
#include <fcntl.h>

// void test(char *file, t_data *img, int i2)
// {
// 	int		fd;
// 	int		lines;
// 	int		i;
// 	char	*test;

// 	lines = 0;;
// 	lines = 0;
// 	fd = open(file, 0);
// 	test = get_next_line(fd);
// 	while(test && ft_strncmp(test, "/* pixels */", 11) != 0)
// 	{
// 		test = get_next_line(fd);
// 		lines ++;
// 	}
// 	while(test)
// 	{
// 		test = get_next_line(fd);
// 		test = ft_substr(test, 1, ft_strlen(test) - 4);

// 		if(ft_strchr(test, ';') != 0 || !test)
// 			break;

// 		i = -1;
// 		while(test[++i])
// 		{
// 			if(test[i] != ' ')
// 				draw_line(img, (t_xy){(i * 3) + 250, (lines * 3) + 250}, (t_xy){(i * 3) + 200, (lines * 3) + 200}, 0x00FF0000);
// 		}
// 			//put_pixel(img, 30 + (i * 2), 30 + (lines * 2), 0x00FF0000);
// 		//put_pixel(img, 30 + iso_map((t_xy){lines, i}).x, 30 + iso_map((t_xy){lines, i}).y, 0x00FF0000);

// 		//put_pixel(img, 30 + (lines * 2), 30 + (i * 2), 0x00FF0000);
// 		//printf("%s\n",test);
		
// 		lines ++;
// 	}
	
// }

int handle_keypress(int keycode, t_frame *current_frame)
{
	t_player *player = current_frame->world->player;
	t_xy new_pos = player->pos;

	if (keycode == 13 && current_frame->world->grid[player->pos.y - 1][player->pos.x] != '1') // 'W' key is pressed
		new_pos.y --;
	else if (keycode == 0 && current_frame->world->grid[player->pos.y][player->pos.x - 1] != '1') // 'A' key is pressed
		new_pos.x --;
	else if (keycode == 1 && current_frame->world->grid[player->pos.y + 1][player->pos.x] != '1') // 'S' key is pressed
		new_pos.y ++;
	else if (keycode == 2 && current_frame->world->grid[player->pos.y][player->pos.x + 1] != '1') // 'D' key is pressed
		new_pos.x ++;
	if(new_pos.x > 0 && new_pos.y > 0)
		player->pos = new_pos;
	
	//current_frame->world->grid[player->pos.y][player->pos.x] = '1';
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

double degrees_to_radians(double degrees) {
    return degrees * M_PI / 180.0;
}

t_xy calculate_endpoint(t_xy start, double angle_rad, int distance) {
    return ((t_xy){start.x + (int)(distance * cos(angle_rad)), start.y + (int)(distance * sin(angle_rad))});
}

void draw_grid_line(char **array, t_xy pos, double angle_deg, int distance, int rows, int cols) {
    double angle_rad = degrees_to_radians(angle_deg);
    t_xy end = calculate_endpoint(pos, angle_rad, distance);
	t_xy difference;

	difference = (t_xy){abs(end.x - pos.x), abs(end.y - pos.y)};

    int step_x = (pos.x < end.x) ? 1 : -1;
    int step_y = (pos.y < end.y) ? 1 : -1;

    int error = difference.x - difference.y;

	printf("\n");
	printf("Start [%d, %d]\n", pos.x, pos.y);
	printf("End [%d, %d]\n", end.x, end.y);
	printf("Difference_XY [%d, %d]\n", difference.x, difference.y);
    while (pos.x != end.x || pos.y != end.y) 
	{
        if (pos.x >= 0 && pos.x < cols && pos.y >= 0 && pos.y < rows)
		{
			if(array[pos.y][pos.x] == '1')
				return;
			else
				array[pos.y][pos.x] = 'H'; 
		}
        if (10 * error > -difference.y) //contains the absolute values. Both error and difference are absolute, hence why they have to look at the smallest small, aka -differencne,y since difference is a absolute value.
		{
            error -= difference.y; //corrected to be nearer to the difference
            pos.x += step_x; //incriment by the step, which is determined by where the endpoint is. Can only be neg or positive because 1 bcoz it cannot increase past that
			//printf("1\n");
        }
        if (10 * error < difference.x) 
		{
            error += difference.x;
            pos.y += step_y;
			//printf("2\n");
        }
    }
    if (end.x >= 0 && end.x < cols && end.y >= 0 && end.y < rows)
        array[end.y][end.x] = 'H';
}


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

int render_grid(t_world *world, t_display *display, char **c, t_grid_display grid, t_data **sprites)
{
	int		tile_x = -1;
	int		tile_y = 0;
	t_data	*b_image;
	t_xy 	b_xy;

	while(++tile_x < 10)
	{
		tile_y = -1;
		while(++tile_y < 10)
		{
			b_image = sprites[2]->img;
			if(c[tile_y][tile_x] == '1')
			{
				b_image = sprites[1]->img;
			}
			if(c[tile_y][tile_x] != 'H')
				mlx_put_image_to_window(display->mlx, display->mlx_win, b_image, 
					iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).x + grid.offset_x,
					iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).y + grid.offset_y - ((c[tile_y][tile_x] == '1') * 20));
			if(tile_x == world->player->pos.x && tile_y == world->player->pos.y)
			{
				world->player->i_pos = iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y});
				render_player(world, (t_xy){world->player->i_pos.x + grid.offset_x + 12, world->player->i_pos.y + grid.offset_y - 25}, display);
			}
			if(c[tile_y][tile_x] == 'S')
			{
				char *message = "460";
				b_xy = (t_xy){
					iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).x + grid.offset_x,
					iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).y + grid.offset_y - 25};
				mlx_put_image_to_window(display->mlx, display->mlx_win, sprites[0]->img, b_xy.x + ((sprites[1]->line_length)/4 - (sprites[0]->line_length)/4) / 2, b_xy.y);
				mlx_string_put(display->mlx,display->mlx_win, b_xy.x + ((sprites[1]->line_length)/4 - (10 * ft_strlen(message))) / 2, b_xy.y, 0x00FFFFFF, message);
			}
		}
	}
	return (1);
}

t_xy	bounce(t_xy pos, t_xy pos2)
{
	return((t_xy){(pos.x + (pos2.x - pos.x)/10), 
				(pos.y + (pos2.y - pos.y)/10)});
}

// int render(t_world *world, t_display *display)
// {
// 	t_object *head = *(world->enemies);
// 	while(head)
// 	{
// 		printf("%s | %s | [pos: %d, %d]\n", head->type, head->data, ((t_enemy *)(head->data))->pos.x, ((t_enemy *)(head->data))->pos.y);
// 		head = head->next;
// 	} 

// 	// world->player->animator.frame_timer = (world->player->animator.frame_timer + 1) % world->player->animator.speed;
// 	// 	if(world->player->animator.frame_timer == 0)
// 	// 		world->player->animator.current_frame = (world->player->animator.current_frame + 1) % 2;
// 	return 1;
// }

int render_next_frame(void *param)
{
	t_frame_data *data = (t_frame_data *)param;
	void *mlx = data->graphic_display->mlx;
	void *mlx_win = data->graphic_display->mlx_win;
	t_world *world = data->world;
	t_display *display = data->graphic_display;
	int mouse_x, mouse_y;
	int i = *(data->i);
	
	(*(data->frame_sec))++;
	if (*(data->frame_sec) > 100)
	{
		mlx_clear_window(mlx, mlx_win);
		*(data->frame_sec) = 0;
		display->camera->pos = bounce(display->camera->pos, world->player->i_pos);
		render_grid(world, display, world->grid, (t_grid_display){21, 21, ((display->width) / 2) - display->camera->pos.x, ((display->height) / 2) - display->camera->pos.y}, data->graphic_display->sprites);
		//draw_grid_line(world->grid, (t_xy){1,1}, i, 20,10,10);
		
		world->player->animator.frame_timer = (world->player->animator.frame_timer + 1) % world->player->animator.speed;
		if(world->player->animator.frame_timer == 0)
			world->player->animator.current_frame = (world->player->animator.current_frame + 1) % 2;
		
		mlx_string_put(mlx,mlx_win, 10, 10, 0x00FF0000, "test");
		(*(data->i))++;
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


void get_objects(char **array, t_animation *animator, t_world *world)
{
	int 		x;
	int 		y;

	t_counter *count = malloc(sizeof(t_counter));
	*count = (t_counter){0,0,0,0,0,0,0};
	t_object	*sentry_list = new_object("null", "data");
	t_object	*object;
	y = -1;
	while(array[++y])
	{
		x = -1;
		while(array[++x])
		{
			if (array[y][x] == 'C')
				count->collectible ++;
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
				*sentry = (t_enemy){(t_xy){x,y}, 10, 5, animator};
				object_add_back(&sentry_list, new_object("enemy", sentry));
			}
			if (array[y][x] == 'P')
				count->player ++;
			if (array[y][x] == 'H')
				count->hole ++;
		}
	}
	printf("player: %d\n",count->player);
	printf("sentry: %d\n",count->sentry);
	printf("coin: %d\n",count->collectible);
	printf("wall: %d\n",count->wall);
	printf("empty: %d\n",count->empty);
	printf("hole: %d\n",count->hole);
	printf("exit: %d\n",count->exit);

	t_object *head = sentry_list;
	while(head)
	{
		printf("%s | %s | [pos: %d, %d]\n", head->type, head->data, ((t_enemy *)(head->data))->pos.x, ((t_enemy *)(head->data))->pos.y);
		head = head->next;
	}
	//world->enemies = &sentry_list;
}

int	main(void)
{
	t_display			*display = graphics_init(1920, 1080);
	t_player 			*player = malloc(sizeof(t_player));
	t_world 			*world = malloc(sizeof(world));

	t_animation	enemy_animation = (t_animation){0,0,5};
	enemy_animation.frames = malloc((sizeof(t_data *) * 2)+ 1);
	enemy_animation.frames[0] = put_img("enemy_idle_1.xpm", display->mlx);
	enemy_animation.frames[0] = put_img("enemy_idle_1.xpm", display->mlx);

	player->pos = (t_xy){1, 1};
	player->i_pos = (t_xy){1, 1};
	player->lives = 4;
	player->animator.current_frame = 0;
	player->animator.frames = malloc((sizeof(t_data *) * 2) + 1);
	player->animator.frames[0] = put_img("player_idle_2.xpm", display->mlx);
	player->animator.frames[1] = put_img("player_idle_1.xpm", display->mlx);
	player->animator.frames[2] = NULL;
	player->animator.speed = 30;

	char c[10][10] = {
	"1011111111",
	"1000000001",
	"1000000001",
	"100000P001",
	"1000001111",
	"100E000101",
	"10000S0101",
	"10000S0101",
	"1000S00001",
	"1001111111"
	};

	char **test = malloc((11 * sizeof(char*)));

	int i = -1;
	while(++i < 10)
	{
		test[i] = malloc(sizeof(char *) + 1);
		ft_strlcpy(test[i], c[i], 11);
	}
	test[i] = NULL;

	get_objects(test, &enemy_animation, world);

	t_enemy	*enemy;
	
	world = &((t_world){player, enemy, NULL, test});
	
	t_frame vars;
	vars.world = world;
	vars.display = display;

	int		sec = 0;
	int		frame_sec = 0;

	t_xy my_pos = {0,0};
	t_xy vector = {1,2};

	int i2 = -1;
	while(++i2 < 5)
	{
		//draw_ray(test, 1,1,26,10,10);
		if(my_pos.y < 5 && my_pos.x < 5)
			test[my_pos.y][my_pos.x] = '1';
	}

	t_frame_data current_frame = (t_frame_data){&frame_sec, world, display, &i};

	mlx_hook(display->mlx_win, 2, 1L << 0, handle_keypress, &vars);
	mlx_loop_hook(display->mlx, render_next_frame, &current_frame);
	mlx_loop(display->mlx);
}