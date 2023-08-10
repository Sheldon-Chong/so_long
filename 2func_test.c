#include <stdio.h>
#include "graphics.h"


int handle_keypress(int keycode, t_player *player)
{
	if (keycode == 13) // 'W' key is pressed
	{
		player->pos.y --;
		printf("w\n");
	}
	else if (keycode == 0) // 'A' key is pressed
	{
		player->pos.x --;
		printf("a\n");
	}
	else if (keycode == 1) // 'S' key is pressed
	{
		player->pos.y ++;
		printf("s\n");
	}
	else if (keycode == 2) // 'D' key is pressed
	{
		player->pos.x ++;
		printf("d\n");
	}
	return (0);
}


typedef struct s_vars {
	t_player *player;
} t_vars;

typedef struct s_game_info {
	t_player *player;
} t_game_info;

typedef struct s_grid_display {
	float space_x;
	float space_y;
	int offset_x;
	int offset_y;
} t_grid_display;

typedef struct s_grid {
	int width;
	int height;
	char **grid;
} t_grid;

typedef struct s_graphic_display {
	int width;
	int height;
	void *mlx;
	void *mlx_win;
} s_graphic_display;

int render_frame(t_grid_display grid, char c[10][10],t_player *player, void *mlx, void *mlx_win)
{
	int tile_x = -1;
	int tile_y = 0;

	while(++tile_x < 10)
	{
		tile_y = -1;
		while(++tile_y < 10)
		{
			if(c[tile_y][tile_x] == 'c')
				mlx_put_image_to_window(mlx, mlx_win, put_img("./tile2.xpm", mlx)->img, 
					iso_map(tile_x * grid.space_x, tile_y * grid.space_y).x + grid.offset_x, iso_map(tile_x * grid.space_x, tile_y * grid.space_y).y + grid.offset_y);
			else
				mlx_put_image_to_window(mlx, mlx_win, put_img("./tile1.xpm", mlx)->img, 
					iso_map(tile_x * grid.space_x, tile_y * grid.space_y).x + grid.offset_x,
					iso_map(tile_x * grid.space_x, tile_y * grid.space_y).y + grid.offset_y);
			if(tile_x == player->pos.x && tile_y == player->pos.y)
				mlx_put_image_to_window(mlx, mlx_win, put_img("./tile1.xpm", mlx)->img, 
					iso_map(tile_x * grid.space_x, tile_y * grid.space_y).x + grid.offset_x,
					iso_map(tile_x * grid.space_x, tile_y * grid.space_y).y + grid.offset_y - 30);
		}
	}
	return (1);
}

int render_grid(t_game_info *game_info, void*mlx, void *mlx_win, char **c)
{
	int tile_x = -1;
	int tile_y = 0;
	t_grid_display grid = (t_grid_display){21,21,300,300};

	while(++tile_x < 10)
	{
		tile_y = -1;
		while(++tile_y < 10)
		{
			if(c[tile_y][tile_x] == 'c')
				mlx_put_image_to_window(mlx, mlx_win, put_img("./tile2.xpm", mlx)->img, 
					iso_map(tile_x * grid.space_x, tile_y * grid.space_y).x + grid.offset_x, iso_map(tile_x * grid.space_x, tile_y * grid.space_y).y + grid.offset_y);
			else
				mlx_put_image_to_window(mlx, mlx_win, put_img("./tile1.xpm", mlx)->img, 
					iso_map(tile_x * grid.space_x, tile_y * grid.space_y).x + grid.offset_x,
					iso_map(tile_x * grid.space_x, tile_y * grid.space_y).y + grid.offset_y);
			if(tile_x == game_info->player->pos.x && tile_y == game_info->player->pos.y)
				mlx_put_image_to_window(mlx, mlx_win, put_img("./tile1.xpm", mlx)->img, 
					iso_map(tile_x * grid.space_x, tile_y * grid.space_y).x + grid.offset_x,
					iso_map(tile_x * grid.space_x, tile_y * grid.space_y).y + grid.offset_y-30);
		}
	}
	return 1;
}

s_graphic_display *graphics_init(int width, int height)
{
	s_graphic_display *ret;

	ret = malloc(sizeof(s_graphic_display));
	void *mlx;
	void *mlx_win;
	
	ret->mlx = mlx_init();
    ret->mlx_win = mlx_new_window(ret->mlx, 1920, 1080, "Hello world!");
    ret->width = width;
    ret->height = height;

	return(ret);
}

int	main(void)
{
	s_graphic_display	*display = graphics_init(1920, 1080);


	t_player *player = malloc(sizeof(t_player));
	
	t_game_info *game_info = malloc(sizeof(game_info));
	game_info = &((t_game_info){player});

	player->pos = (t_xy){0, 0};
	player->lives = 4;
	
	t_grid	*grid2;
	grid2 = malloc(sizeof(grid2));

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
		
	int tile_x = -1;
	int tile_y = 0;

	render_grid(game_info, display->mlx, display->mlx_win, test);
	
	t_vars vars;
	vars.player = player;

	mlx_hook(display->mlx_win, 2, 1L << 0, handle_keypress, &vars);
	mlx_loop(display->mlx);
}