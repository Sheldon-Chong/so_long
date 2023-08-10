#include <mlx.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
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


typedef struct	frame_data {
	int *frame_sec;
	void *mlx;
	void *mlx_win;
	int *i;
} t_frame_data;

void draw_square(t_data *img, int width, int height, int x_pos)
{
	int x = -1;
	int y;
	
	while(++x < width)
	{
		y = 0;
		while(++y < height)
			my_mlx_pixel_put(img, x,y, 0x00FF0000);
	}
}


int render_next_frame(void *param)
{
    t_frame_data *data = (t_frame_data *)param;
    void *mlx = data->mlx;
	void *mlx_win = data->mlx_win;

    t_data *img = new_img(mlx);
    (*(data->frame_sec))++;
    if (*(data->frame_sec) > 100)
    {
        printf("[%d]\n", *(data->i));
        *(data->frame_sec) = 0;

        draw_square(img, *(data->i) * 2, 10, 0);
		mlx_put_image_to_window(mlx, mlx_win, img->img, 0,0);
		(*(data->i))++;
    }
    return 1;
}

int main()
{
	void	*mlx;
	void	*mlx_win;
	t_data	*img;
	int		frame_sec = 0;
	int		i = 0;
	

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1920, 1080, "Hello world!");
	t_frame_data test = (t_frame_data){&frame_sec, mlx, mlx_win, &i};
	mlx_loop_hook(mlx, render_next_frame, &test);
	// img = new_img(mlx);
	// my_mlx_pixel_put(img, 5,5, 0x00FF0000);
	// mlx_put_image_to_window(mlx, mlx_win, img->img, 20,20);

	mlx_loop(mlx);
}