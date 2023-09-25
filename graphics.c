/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:46:43 by shechong          #+#    #+#             */
/*   Updated: 2023/09/18 13:03:06 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

t_data	*new_img(void *mlx, int x, int y)
{
	t_data	*img;

	img = malloc(sizeof(t_data));
	img->img = mlx_new_image(mlx, x, y);
	img->addr = mlx_get_data_addr(img->img, &(img->bits_per_pixel),
			&(img->line_length), &(img->endian));
	return (img);
}

t_data	*put_img(char *image, void *mlx)
{
	int		img_width;
	int		img_height;
	t_data	*img;

	img = malloc(sizeof(t_data));
	img->img = mlx_xpm_file_to_image(mlx, image, &img_width, &img_height);
	printf("%s | %p, %p\n", image, img, img->img);
	if (img->img == NULL)
	{
		free(img);
		return (NULL);
	}
	img->addr = mlx_get_data_addr(img->img, &(img->bits_per_pixel),
			&(img->line_length), &(img->endian));
	return (img);
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}
