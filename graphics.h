#ifndef GRAPHICS
# define GRAPHICS

#include <mlx.h>
#include <stdlib.h>
#include <math.h>
#include "libft/libft.h"

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct	s_xy {
	int		x;
	int		y;
}				t_xy;

typedef struct	s_player 
{
	t_xy	pos;
	int		lives;
}				t_player; 

// ISOMEtRIC

t_xy iso_map(int x, int y)
{
	return (t_xy){(x - y) * 1.4, (x + y) / 1.4};
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