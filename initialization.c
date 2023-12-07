/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:46:23 by shechong          #+#    #+#             */
/*   Updated: 2023/11/02 19:57:46 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

t_display	display_init(int width, int height)
{
	t_camera	*camera;
	t_display	ret;

	camera = malloc(sizeof(t_camera));
	*camera = (t_camera){(t_xy){1, 1}, (t_xy){1, 1}};
	ret.dimensions = (t_xy){width, height};
	ret.mlx = mlx_init();
	ret.mlx_win = mlx_new_window(ret.mlx, width, height, "Hello world!");
	ret.width = width;
	ret.height = height;
	ret.camera = camera;
	ret.animations = NULL;
	ret.anim_spritesheet = NULL;
	ret.grid_display = malloc(sizeof(t_grid_d));
	*(ret.grid_display) = (t_grid_d){1, 1, 1, 1};
	ret.mouse = (t_xy){0, 0};
	ret.sprites = malloc(sizeof(t_data *) * 6);
	ret.sprites[1] = img_from_path("assets/Wall.xpm", ret.mlx);
	ret.sprites[2] = img_from_path("assets/tile_black.xpm", ret.mlx);
	ret.sprites[3] = img_from_path("assets/tile_white.xpm", ret.mlx);
	ret.sprites[4] = img_from_path("assets/Exit.xpm", ret.mlx);
	ret.sprites[5] = NULL;

	return (ret);
}

t_data	**frames(char *frames, char *directory, t_display *display)
{
	t_data	**array;
	char	**char_array;
	char	*path;
	int		i;

	char_array = ft_split(frames, ',');
	i = 0;
	while (char_array[i])
		i ++;
	array = malloc(sizeof(t_data *) * (i + 1));
	array[i - 1] = NULL;
	i = -1;
	while (char_array[++i])
	{
		path = ft_strjoin(directory, char_array[i]);
		array[i] = img_from_path(path, display->mlx);
		free(path);
	}
	ft_free_array((void *)(char_array), 0);
	array[i] = NULL;
	return (array);
}

void	animation_init(t_world *world, t_display *display)
{
	t_animator	*coin_animation;
	t_animator	*enemy_animation;

	coin_animation = malloc(sizeof(t_animator));
	enemy_animation = malloc(sizeof(t_animator));
	*coin_animation = (t_animator){0, 0, 30,
		frames("collectible_1.xpm,collectible_4.xpm", "assets/", display)};
	*enemy_animation = (t_animator){0, 0, 5,
		frames("sentry.xpm,sentry.xpm", "assets/", display)};
	world->player->animator = (t_animator){0, 0, 30,
		frames("player.xpm,player_2.xpm", "assets/", display)};
	append(&display->anim_spritesheet,
		new_obj("coin animation", coin_animation));
	append(&display->anim_spritesheet,
		new_obj("enemy animation", enemy_animation));
	append(&display->animations, new_obj("animation", coin_animation));
	append(&display->animations, new_obj("animation", enemy_animation));
	append(&display->animations, new_obj("player animation",
			&(world->player->animator)));
}

t_world	*world_init(char *map)
{
	t_world		*world;
	t_player	*player;

	player = malloc(sizeof(t_player));
	world = malloc(sizeof(t_world));
	world->count = (t_counter){0, 0, 0, 0, 0};
	*world = ((t_world){player, NULL, NULL, scan_map(world, map),
			NULL, world->dimensions, (t_counter){0, 0, 0, 0, 0}});
	*player = (t_player){(t_xy){1, 1}, (t_xy){1, 1}, (t_animator){0, 0, 0, 0}};
	return (world);
}

t_data	*get_frame(void *t, int c)
{
	if (c == 0)
		return (((t_coin *)t)->animator->frames
			[((t_coin *)t)->animator->current_frame]);
	if (c == 1)
		return (((t_enemy *)t)->animator->frames
			[((t_enemy *)t)->animator->current_frame]);
	return (NULL);
}
