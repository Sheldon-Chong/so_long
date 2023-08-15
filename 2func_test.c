#include <stdio.h>
#include "graphics.h"
#include <fcntl.h>


int handle_keypress(int keycode, t_frame *frame)
{
	t_player *player = frame->world->player;
	if (keycode == 13 && frame->world->grid[player->pos.y - 1][player->pos.x] != '1') // 'W' key is pressed
		player->pos.y --;
	else if (keycode == 0 && frame->world->grid[player->pos.y][player->pos.x - 1] != '1') // 'A' key is pressed
		player->pos.x --;
	else if (keycode == 1 && frame->world->grid[player->pos.y + 1][player->pos.x] != '1') // 'S' key is pressed
		player->pos.y ++;
	else if (keycode == 2 && frame->world->grid[player->pos.y][player->pos.x + 1] != '1') // 'D' key is pressed
		player->pos.x ++;
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



int render_player(t_world *world, t_xy pos, t_graphic_display *display)
{
	void	*mlx	 = display->mlx;
	void	*mlx_win = display->mlx_win;
	t_player *player = world->player;
	char	**grid = world->grid;

	mlx_put_image_to_window(mlx, mlx_win, put_img("./p1.xpm", mlx)->img, pos.x, pos.y);	
	return 1;
}

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

int render_grid(t_world *world, t_graphic_display *display, char **c, t_grid_display grid)
{
	int		tile_x = -1;
	int		tile_y = 0;
	void	*mlx = display->mlx;
	void	*mlx_win = display->mlx_win;
	t_data	*image;

	while(++tile_x < 10)
	{
		tile_y = -1;
		while(++tile_y < 10)
		{
			image = put_img("./tile2.xpm", mlx)->img;
			if(c[tile_y][tile_x] == '1')
				image = put_img("./tile1.xpm", mlx)->img;
			mlx_put_image_to_window(mlx, mlx_win, image, 
				iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).x + grid.offset_x,
				iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).y + grid.offset_y);
			if(tile_x == world->player->pos.x && tile_y == world->player->pos.y)
			{
				world->player->i_pos = iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y});
				render_player(world, (t_xy){world->player->i_pos.x + grid.offset_x + 12, world->player->i_pos.y + grid.offset_y - 25}, display);
			}
			free(image);
		}
	}
	return (1);
}

t_xy	bounce(t_xy pos, t_xy pos2)
{
	return((t_xy){(pos.x + (pos2.x - pos.x)/10), 
				(pos.y + (pos2.y - pos.y)/10)});
}

int render_next_frame(void *param)
{
	t_frame_data *data = (t_frame_data *)param;
	void *mlx = data->graphic_display->mlx;
	void *mlx_win = data->graphic_display->mlx_win;
	t_world *world = data->world;
	t_graphic_display *display = data->graphic_display;
	int mouse_x, mouse_y;
	int i = *(data->i);

	t_data *img = new_img(mlx, 300, 300);
	(*(data->frame_sec))++;
	if (*(data->frame_sec) > 100)
	{
		mlx_clear_window(mlx, mlx_win);
		//test("player_1.xpm", img, (*(data->i)));
		*(data->frame_sec) = 0;
		draw_line(img, (t_xy){0,0}, (t_xy){10,100}, 0x00FF0000);
		mlx_put_image_to_window(mlx, mlx_win, img->img, 0, 0);
		display->camera->pos = bounce(display->camera->pos, world->player->i_pos);
		render_grid(world, display, world->grid, (t_grid_display){21, 21, ((display->width) / 2) - display->camera->pos.x, ((display->height) / 2) - display->camera->pos.y});
		mlx_string_put(mlx,mlx_win,10,10, 0x00FF0000, "test");
		(*(data->i))++;
	}
	free(img);
	return 1;
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

void get_objects(char **array)
{
	int x;
	int y;

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
				object_add_back(&sentry_list, new_object("enemy", "data"));
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
		printf("%s | %s\n", head->type, head->data);
		head = head->next;
	}
}

int	main(void)
{
	t_graphic_display	*display = graphics_init(1920, 1080);
	t_player 			*player = malloc(sizeof(t_player));
	t_world 			*world = malloc(sizeof(world));

	player->pos = (t_xy){1, 1};
	player->lives = 4;

	char c[10][10] = {
	"1011111111",
	"1SS0000001",
	"1000000001",
	"100000P001",
	"1000010001",
	"100E000001",
	"1000000001",
	"100CC00H01",
	"1000000001",
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

	get_objects(test);

	t_enemy	*enemy;
	
	world = &((t_world){player, enemy, test});
	render_grid(world, display, test, (t_grid_display){21, 21, 300, 300});
	
	t_frame vars;
	vars.world = world;
	vars.display = display;

	int		sec = 0;
	int		frame_sec = 0;
	t_frame_data frame = (t_frame_data){&frame_sec, world, display, &i};
	mlx_hook(display->mlx_win, 2, 1L << 0, handle_keypress, &vars);
	mlx_loop_hook(display->mlx, render_next_frame, &frame);
	mlx_loop(display->mlx);
}