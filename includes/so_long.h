/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 20:06:01 by shechong          #+#    #+#             */
/*   Updated: 2024/01/25 10:07:20 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <mlx.h>
# include <stdlib.h>
# include <math.h>
# include "../libft/libft.h"
# include <stdio.h>
# include <sys/fcntl.h>
# include <stdbool.h>

//screen
# define SCREEN_WIDTH 1920
# define SCREEN_HEIGHT 1080

//colors
# define COLOR_BLACK 0x00000000
# define C_TRANSPARENT 0xFFFF0000
# define C_MLX_TRANSPARENT -16777216

//object sprites
# define SENTRY_FOV_COLOR 0x30eeff
# define SENTRY_MAX_ALERT 100
# define SENTRY_TURN_RATE 100
# define SENTRY_SPRITES "sentry.xpm"
# define COLLECTIBLE_SPRITES "collectible_1.xpm,collectible_2.xpm"
# define PLAYER_SPRITES "player.xpm,player_2.xpm"
# define SENTRY_MOVEMENT_DAY 30

//map sprites
# define WALL_SPRITE "assets/wall.xpm"
# define FLOOR_1_SPRITE "assets/tile_black.xpm"
# define FLOOR_2_SPRITE "assets/tile_white.xpm"
# define EXIT_SPRITE "assets/exit.xpm"

# define VALID_CHAR "1P2CESH0\n"

//hooks
# define ON_MOUSE_MOTION 6
# define ON_KEY_PRESS 2
# define ON_DESTROY 17
# define ON_ESCAPE 53

//keys
# define LINUX_W 13
# define LINUX_A 0
# define LINUX_S 1
# define LINUX_D 2

//general struct to store coordinates
typedef struct s_xy
{
	int		x;
	int		y;
}	t_xy;

//general utility struct to keep track of time
typedef struct s_timer
{
	int	stopper;
	int	elapsed;
}	t_timer;

typedef struct s_object
{
	char			*type;
	void			*data;
	struct s_object	*next;
}	t_object;

typedef struct camera
{
	t_xy		pos;
	t_xy		pos_goto;
}	t_camera;

typedef struct s_img
{
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			width;
	int			height;
}	t_img;

//general util to store information of animator
typedef struct s_animator
{
	int			frame_index;
	int			frame_timer;
	int			speed;
	t_img		**frames;
	t_img		*current_frame;
}	t_animator;

typedef struct s_enemy
{
	t_xy		pos;
	int			current_angle;
	int			final_angle;
	int			hp;
	t_animator	animator;
	bool		player_found;
	int			alert;
	t_xy		current_pos;
	t_timer		time;
}	t_sentry;

//for enemy raycasting
typedef struct s_ray
{
	double		angle_rad;
	t_xy		end;
	t_xy		dif;
	int			step_x;
	int			step_y;
	int			error;
}	t_ray;

typedef struct s_collectible
{
	t_xy		pos;
	t_animator	animator;
}	t_collectible;

typedef struct s_player
{
	t_xy		pos;
	t_xy		mapped_pos;
	t_animator	animator;
}	t_player;

typedef struct s_grid_display
{
	float		space_x;
	float		space_y;
	int			offset_x;
	int			offset_y;
}	t_grid_d;

typedef struct s_display
{
	void		*mlx;
	void		*mlx_win;
	t_img		**sprites;
	t_camera	*camera;
	t_xy		mouse;
	t_xy		dimensions;
	t_grid_d	*grid_display;
	t_img		*img;
}	t_display;

//keep count of different items in the map
typedef struct s_counter
{
	int	player;
	int	sentry;
	int	collectible;
	int	moves;
	int	exit;
}	t_counter;

//a tile of a map and the data associated with it
typedef struct s_tile
{
	char	type;
	void	*data;
}	t_tile;

typedef struct s_world
{
	t_player	*player;
	t_object	*enemies;
	t_object	*collectibles;
	char		**grid;
	t_tile		**tgrid;
	t_xy		dimensions;
	t_counter	count;
	bool		has_collected_all_collectibles;
	int			collectibles_collected;
}	t_world;

typedef struct frame_data
{
	int			*frame_sec;
	t_world		*world;
	t_display	*display;
}	t_frame;

//enemy.c
void			sentry_move(t_sentry *enemy, t_xy pos, t_world *world);
int				sentry_decide_move_vect(t_world *world, t_sentry *enemy);
int				sentry_fov_raycast(t_world *world, t_sentry *enemy);
void			enemy_track(t_world *world,
					t_display *display, t_sentry *enemy);
void			update_sentries(t_world *world, t_display *display);

//free.c
void			free_world_display(t_world *world, t_display *display);
void			ft_free_objects(t_object **head);
void			img_destroy(void *mlx, t_img *img);
void			free_obj_list(t_object *object);
void			free_animations(t_world *world, t_display *display);

//graphics.c
t_img			*empty_img(void *mlx, int x, int y);
t_img			*img_from_path(char *image, void *mlx);

//initialization.c
t_display		display_init(int width, int height);			//test
t_img			**frames(char *frames, char *directory, t_display *display);
void			animation_init(t_world *world, t_display *display);
t_world			*world_init(char *map, t_display *display);

//hooks.c
int				handle_keypress(int keycode, t_frame *frame_index);
void			render_ui(t_world *world, t_display *display);
int				render_frame(void *param);
int				mouse(int x, int y, void *param);

//main.c
void			print_list(t_object *start);
int				exit_hook(void *param);

//maths.c
int				ran_int(int min, int max);
double			deg_to_rad(double degrees);
t_xy			move_in_dir(t_xy currentPos, double direction, double distance);
t_xy			polar_to_vec2(t_xy start, double angle_rad, int distance);
int				vec2_to_angle(t_xy point1, t_xy point2);

//objects.c
t_object		*new_obj(char *type, void *data);
t_object		*append(t_object **head, t_object *object);
t_sentry		*new_sentry(t_display *display, t_xy pos);
t_collectible	*new_collectible(t_display *display, t_xy pos);

//parsing.c
int				find_holes(char **array, int rows);
void			count_items(char *array, t_world *world);
t_tile			**char2tile(t_world *world, int row_count, t_display *display);
char			**scan_map(t_world *world, char *file);

//parsing2.c
int				find_exit(char **c, t_world *world);
int				count_newline(char *filename, t_world *world);

//pixels.c
void			draw_rect(t_img *img, t_xy dimensions, t_xy pos, int color);
void			put_pixel(t_img *data, int x, int y, int color);
void			draw_line(t_img *img, t_xy start, t_xy end, int color);

//positioning.c
int				center(t_img *image, t_img *image2);
t_xy			interpolate(t_xy pos, t_xy pos2, int value);
t_xy			iso_map(t_xy pos);

//printing.c
void			print_2d_tiles(t_tile **c);
void			print_char_array(char **c);

//ray_casting.c
void			ray_init(t_ray *ray, t_xy pos, double angle_deg, int distance);
int				ray_cast(t_world *world, t_xy pos,
					double angle_deg, int distance);

//rendering_objects.c
t_xy			interpolate(t_xy pos, t_xy pos2, int value);
void			put_pixel(t_img *data, int x, int y, int color);
t_img			*empty_img(void *mlx, int x, int y);
t_img			*img_from_path(char *image, void *mlx);

void			draw_line(t_img *img, t_xy start, t_xy end, int color);
void			draw_rect(t_img *img, t_xy dimensions, t_xy pos, int color);

void			print_char_array(char **c);
void			print_2d_tiles(t_tile	**c);
void			count_items(char *array, t_world *world);
char			**map2char_array(char *file, int rows);
t_tile			**char2tile(t_world *world, int row_count, t_display *display);

void			ray_init(t_ray *ray, t_xy pos, double angle_deg, int distance);
int				ray_cast(t_world *world, t_xy pos, double angle_deg,
					int distance);

int				render_floor(t_world *world,
					t_display *display, t_img **sprites);
int				render_sentry(t_display *display, t_sentry *enemy);
void			render_world(t_world *world, t_display *display);

void			update_sentries(t_world *world, t_display *display);
int				update_all_animators(t_display *display, t_world *world);

int				draw_fov(t_sentry *enemy, t_display *display,
					t_img *char_array);

int				exit_game(t_world *world, t_display *display);
int				mouse(int x, int y, void *param);
int				render_frame(void *param);

int				exit_game(t_world *world, t_display *display);
void			print_statistics(t_world *world);
void			print_2d_tiles(t_tile **c);
t_xy			render_tile(t_display *display,
					t_img *b_image, t_xy pos, t_xy mod);
int				render_player(t_world *world, t_display *display);
void			print_end_screen(t_world *world);

int				img_impose(t_img *canvas, t_img *img, t_xy start, t_xy scaling);
int				clear_img(t_img *img);

int				get_color(t_img *img, int x, int y);

void			render_obj(t_world *world, t_display *display, t_xy tile);

#endif