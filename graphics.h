#ifndef GRAPHICS
# define GRAPHICS

#include <mlx.h>
#include <stdlib.h>
#include <math.h>
#include "libft/libft.h"

typedef struct	s_xy {
	int		x;
	int		y;
}				t_xy;

typedef struct	s_player 
{
	t_xy	pos;
	t_xy	i_pos;
	int		lives;
}				t_player; 

typedef struct s_graphic_display {
	int width;
	int height;
	void *mlx;
	void *mlx_win;
} t_graphic_display;

typedef struct s_frame {
	t_player			*player;
	t_graphic_display	*graphics;
	char				**grid;
} t_frame;

typedef struct s_game_info {
	t_player *player;
	char	**grid;
} t_game_info;


typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct	frame_data {
	int 		*frame_sec;
	t_game_info *game_info;
	t_graphic_display *graphic_display;
	int 		*i;
} t_frame_data;

// ISOMEtRIC

t_xy iso_map(t_xy pos)
{
	return (t_xy){(pos.x - pos.y) * 1.4, (pos.x + pos.y) / 1.4};
}


t_xy map_iso(t_xy pos) {
    float tempX = (pos.x * 0.7 + pos.y * 0.7) / 2.0;
    float tempY = (pos.y * 0.7 - pos.x * 0.7) / 2.0;
    return (t_xy){tempX, tempY};
}


t_graphic_display *graphics_init(int width, int height)
{
	t_graphic_display *ret;

	ret = malloc(sizeof(t_graphic_display));
	void *mlx;
	void *mlx_win;
	
	ret->mlx = mlx_init();
	ret->mlx_win = mlx_new_window(ret->mlx, width, height, "Hello world!");
	ret->width = width;
	ret->height = height;

	return(ret);
}

// RENDERING

void	put_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

t_data	*new_img(void *mlx)
{
	t_data	*img;

	img = malloc(sizeof(t_data));
	img->img = mlx_new_image(mlx, 600, 300);
	img->addr = mlx_get_data_addr(img->img, &(img->bits_per_pixel), &(img->line_length),
								&(img->endian));
	return(img);
}

t_data *put_img(char *image, void *mlx)
{
	int		img_width;
	int		img_height;

	t_data *img = malloc(sizeof(t_data));
	img->img = mlx_xpm_file_to_image(mlx, image, &img_width, &img_height);
	img->addr = mlx_get_data_addr(img->img, &(img->bits_per_pixel), &(img->line_length),
								&(img->endian));
	return(img);
}

// FRAME



// COLORING

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

# endif