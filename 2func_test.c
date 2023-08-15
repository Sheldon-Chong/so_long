#include <stdio.h>
#include "graphics.h"
#include <fcntl.h>


int handle_keypress(int keycode, t_frame *player)
{
	if (keycode == 13) // 'W' key is pressed
	{
		printf("%p", player);
		player->player->pos.y --;
		printf("w\n");
	}
	else if (keycode == 0) // 'A' key is pressed
	{
		player->player->pos.x --;
		printf("a\n");
	}
	else if (keycode == 1) // 'S' key is pressed
	{
		player->player->pos.y ++;
		printf("s\n");
	}
	else if (keycode == 2) // 'D' key is pressed
	{
		player->player->pos.x ++;
		printf("d\n");
	}
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
			if(c[tile_y][tile_x] == 'c')
				image = put_img("./tile2.xpm", mlx)->img;
			else
				image = put_img("./tile1.xpm", mlx)->img;
			mlx_put_image_to_window(mlx, mlx_win, image, 
				iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).x + grid.offset_x,
				iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).y + grid.offset_y);
			if(tile_x == world->player->pos.x && tile_y == world->player->pos.y)
			{
				world->player->i_pos = iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y});
				render_player(world, (t_xy){world->player->i_pos.x + grid.offset_x+12, world->player->i_pos.y + grid.offset_y - 25}, display);
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

int	main(void)
{
	t_graphic_display	*display = graphics_init(1920, 1080);
	t_player 			*player = malloc(sizeof(t_player));
	t_world 		*world = malloc(sizeof(world));

	player->pos = (t_xy){0, 0};
	player->lives = 4;

	char c[10][10] = {
	"cffffffffc",
	"cffffffffc",
	"cffffffffc",
	"cffffffffc",
	"cffffffffc",
	"cffffffffc",
	"cffffffffc",
	"cffffffffc",
	"cffffffffc",
	"cfffcffffc"
	};

	char **test = malloc(10 * sizeof(char*));

	int i = -1;
	while(++i < 10)
	{
		test[i] = malloc(sizeof(char *) + 1);
		ft_strlcpy(test[i], c[i], 11);
	}

	t_enemy	*enemy;
	
	world = &((t_world){player, enemy, test});
	render_grid(world, display, test, (t_grid_display){21, 21, 300, 300});
	
	t_frame vars;
	vars.player = player;
	vars.graphics = display;

	int		sec = 0;
	int		frame_sec = 0;
	t_frame_data frame = (t_frame_data){&frame_sec, world, display, &i};
	mlx_hook(display->mlx_win, 2, 1L << 0, handle_keypress, &vars);
	mlx_loop_hook(display->mlx, render_next_frame, &frame);
	mlx_loop(display->mlx);
}