/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphic_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:46:43 by shechong          #+#    #+#             */
/*   Updated: 2024/01/04 11:36:11 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

t_img	*empty_img(void *mlx, int x, int y)
{
	t_img	*img;

	img = malloc(sizeof(t_img));
	img->img = mlx_new_image(mlx, x, y);
	img->addr = mlx_get_data_addr(img->img, &(img->bits_per_pixel),
			&(img->line_length), &(img->endian));
	img->width = x;
	img->height = y;
	return (img);
}

t_img	*img_from_path(char *image, void *mlx)
{
	int		img_width;
	int		img_height;
	t_img	*img;

	img = malloc(sizeof(t_img));
	img->img = mlx_xpm_file_to_image(mlx, image, &img->width, &img->height);
	if (img->img == NULL)
	{
		free(img);
		return (NULL);
	}
	img->addr = mlx_get_data_addr(img->img, &(img->bits_per_pixel),
			&(img->line_length), &(img->endian));
	return (img);
}

int	img_impose(t_img *canvas, t_img *img, t_xy start, t_xy scaling)
{
	t_xy	pix;
	int		color;

	pix = (t_xy){-1, -1};
	scaling.x = img->width * scaling.x;
	scaling.y = img->height * scaling.y;
	while (++pix.y < scaling.y)
	{
		pix.x = -1;
		while (++pix.x < scaling.x)
		{
			if (start.x + pix.x > 0 && start.y + pix.y > 0)
			{
				if (pix.x + start.x > SCREEN_WIDTH
					|| pix.y + start.y > SCREEN_HEIGHT)
					break ;
				if (scaling.x == img->width && scaling.y == img->height)
					color = get_color(img, pix.x, pix.y);
				else
					color = get_color(img, (int)(((double)pix.x \
					/ (double)scaling.x) * img->width), \
					(int)(((double)pix.y / (double)scaling.y) * img->height));
				if (color != C_MLX_TRANSPARENT && color != C_TRANSPARENT)
					put_pixel(canvas, pix.x + start.x, pix.y + start.y, color);
			}
		}
	}
	return (1);
}
