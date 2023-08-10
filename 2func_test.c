#include <stdio.h>
#include "graphics.h"


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

int render_grid(t_game_info *game_info, t_graphic_display *display, char **c, t_grid_display grid)
{
	int		tile_x = -1;
	int		tile_y = 0;
	void	*mlx = display->mlx;
	void	*mlx_win = display->mlx_win;

	while(++tile_x < 10)
	{
		tile_y = -1;
		while(++tile_y < 10)
		{
			if(c[tile_y][tile_x] == 'c')
				mlx_put_image_to_window(mlx, mlx_win, put_img("./tile2.xpm", mlx)->img, 
					iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).x + grid.offset_x, iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).y + grid.offset_y);
			else
				mlx_put_image_to_window(mlx, mlx_win, put_img("./tile1.xpm", mlx)->img, 
					iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).x + grid.offset_x,
					iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).y + grid.offset_y);
			if(tile_x == game_info->player->pos.x && tile_y == game_info->player->pos.y)
			{
				game_info->player->i_pos = (t_xy){
					iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).x,
					iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).y};
				mlx_put_image_to_window(mlx, mlx_win, put_img("./tile1.xpm", mlx)->img, game_info->player->i_pos.x + grid.offset_x, game_info->player->i_pos.y + grid.offset_y - 30);	
			}
		}
	}
	return 1;
}

void draw_square(t_data *img, int width, int height, int x_pos, int y_pos)
{
	int x = -1;
	int y;
	
	while(++x < width)
	{
		y = 0;
		while(++y < height)
			put_pixel(img, x+x_pos,y+y_pos, 0x00FF0000);
	}
}

void draw_line(t_data *img, t_xy start, t_xy end, int color) {
    int dx = end.x - start.x;
    int dy = end.y - start.y;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float xIncrement = (float)dx / steps;
    float yIncrement = (float)dy / steps;

    float x = start.x;
    float y = start.y;

    for (int i = 0; i <= steps; i++) {
        put_pixel(img, (int)x, (int)y, color);
        x += xIncrement;
        y += yIncrement;
    }
}

float InOutQuadBlend(float t)
{
    if(t <= 0.5f)
        return 2.0f * t * t;
    t -= 0.5f;
    return (2.0f * t * (1.0f - t) + 0.5f);
}

float lerp(int t, int x, int x2)
{
    return (x + ((x2 - x) * t));
}

t_xy lerp_xy(float t, t_xy p1, t_xy p2) {
    t_xy result;
    result.x = p1.x + (int)((p2.x - p1.x) * t);
    result.y = p1.y + (int)((p2.y - p1.y) * t);
    return result;
}

t_xy	bounce(t_xy pos, t_xy pos2)
{
	// if ((pos2.x - pos.x) < 1)
		
	// if ((pos2.y - pos.y) < 1)
		
	return((t_xy){(pos2.x - pos.x)/3, (pos2.y - pos.x)/3});
}

int render_next_frame(void *param)
{
    t_frame_data *data = (t_frame_data *)param;
    void *mlx = data->graphic_display->mlx;
	void *mlx_win = data->graphic_display->mlx_win;
	t_game_info *game_info = data->game_info;
	t_graphic_display *display = data->graphic_display;
	int mouse_x, mouse_y;
	int i = *(data->i);

    t_data *img = new_img(mlx);
    (*(data->frame_sec))++;
    if (*(data->frame_sec) > 100)
    {
		mlx_clear_window(mlx, mlx_win);
        *(data->frame_sec) = 0;
		mlx_put_image_to_window(mlx, mlx_win, img->img, 0,0);
		display->camera->pos = bounce(display->camera->pos, game_info->player->i_pos);
		render_grid(game_info, display, game_info->grid, (t_grid_display){21, 21, ((display->width) / 2) - display->camera->pos.x, ((display->height) / 2) - display->camera->pos.y});
		(*(data->i))++;
	}
	return 1;
}


int	main(void)
{
	t_graphic_display	*display = graphics_init(1920, 1080);
	t_player *player = malloc(sizeof(t_player));
	
	t_game_info *game_info = malloc(sizeof(game_info));
	

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
	
	game_info = &((t_game_info){player, test});

	render_grid(game_info, display, test, (t_grid_display){21, 21, 300, 300});
	
	t_frame vars;
	vars.player = player;
	vars.graphics = display;

	

	int sec = 0;
	int		frame_sec = 0;
	t_frame_data frame = (t_frame_data){&frame_sec, game_info, display, &i};
	printf("%p\n", player);
	mlx_hook(display->mlx_win, 2, 1L << 0, handle_keypress, &vars);
	mlx_loop_hook(display->mlx, render_next_frame, &frame);
	mlx_loop(display->mlx);
}