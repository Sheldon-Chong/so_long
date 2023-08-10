#include <mlx.h>
#include <stdlib.h>

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;


≈

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
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



t_data	*put_line(void *mlx)
{
	t_data	*img;

	img = malloc(sizeof(t_data));
	img->img = mlx_new_image(mlx, 600, 300);
	img->addr = mlx_get_data_addr(img->img, &(img->bits_per_pixel), &(img->line_length),
								&(img->endian));
	my_mlx_pixel_put(img, 5, 5, 0x00FF0000);
	return(img);
}

int	main(void)
{
	void	*mlx;
	void	*mlx_win;
	t_data	img;
	int		img_width;
	int		img_height;
	
	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1920, 1080, "Hello world!");
	
	mlx_put_image_to_window(mlx, mlx_win, put_line(mlx)->img, 0, 0);
	mlx_put_image_to_window(mlx, mlx_win, put_img("./Val.xpm", mlx)->img, 101, 0);

	
	mlx_loop(mlx);
}