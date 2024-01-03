/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 20:06:01 by shechong          #+#    #+#             */
/*   Updated: 2024/01/03 13:43:25 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICS_H
# define GRAPHICS_H

# include <mlx.h>
# include <stdlib.h>
# include <math.h>
# include "libft/libft.h"
# include <stdio.h>
# include <sys/fcntl.h>
# define ON_DESTROY 17
# define LINUX_W 13
# define LINUX_A 0
# define LINUX_S 1
# define LINUX_D 2
# define SCREEN_WIDTH 1920
# define SCREEN_HEIGHT 1080
# define COLOR_BLACK 0x00000000
# define COLOR_WHITE 0x00FFFFFF
# define COLOR_RED 0x00FF0000
# define COLOR_GREEN 0x0000FF00
# define COLOR_BLUE 0x000000FF
# define COLOR_YELLOW 0x00FFFF00
# define COLOR_MAGENTA 0x00FF00FF
# define COLOR_CYAN 0x0000FFFF
# define COLOR_TRANSPARENT 0xFFFF0000

typedef struct s_xy
{
	int		x;
	int		y;
}	t_xy;

typedef struct s_timer {
	int	stopper;
	int	elapsed;
}	t_timer;

typedef struct s_object
{
	char			*type;
	void			*data;
	struct s_object	*next;
}	t_object;

typedef struct camera {
	t_xy	pos;
	t_xy	pos_goto;
}	t_camera;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}				t_img;

typedef struct s_animator
{
	int		frame_index;
	int		frame_timer;
	int		speed;
	t_img	**frames;
	t_img	*current_frame;
}	t_animator;

typedef struct s_enemy
{
	t_xy		pos;			
	int			current_angle;			
	int			final_angle;	
	int			hp;				
	t_animator	*animator;	
	int			player_found;
	int			alert;
	t_xy		current_pos;
	t_object	*timers;
}	t_enemy;

typedef struct s_collectible
{
	t_xy		pos;
	t_animator	animator;
}	t_coin;

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
	int			width;
	int			height;
	void		*mlx;
	void		*mlx_win;
	t_img		**sprites;
	t_camera	*camera;
	t_object	*animations;
	t_object	*anim_spritesheet;
	t_xy		mouse;
	t_xy		dimensions;
	t_grid_d	*grid_display;
	t_img		*img;
}	t_display;

typedef struct t_counter{
	int	player;
	int	sentry;
	int	collectible;
	int	moves;
	int	exit;
}	t_counter;

typedef struct s_tile
{
	char	type;
	void	*data;
}	t_tile;

typedef struct s_world
{
	t_player	*player;
	t_object	*enemies;
	t_object	*coins;
	char		**grid;
	t_tile		**tgrid;
	t_xy		dimensions;
	t_counter	count;
}	t_world;

typedef struct frame_data
{
	int			*frame_sec;
	t_world		*world;
	t_display	*display;
}	t_frame;

typedef struct s_ray
{
	double	angle_rad;
	t_xy	end;
	t_xy	dif;
	int		step_x;
	int		step_y;
	int		error;
}	t_ray;

//enemy.c
void		enemy_move(t_enemy *enemy, t_xy pos, t_world *world);
int			decide_enemy_movement(t_world *world, t_enemy *enemy);
int			enemy_search4player(t_world *world, t_enemy *enemy);
void		enemy_track(t_world *world, t_display *display, t_enemy *enemy);
void		update_enemies(t_world *world, t_display *display);

//free.c
void		free_world_display(t_world *world, t_display *display);
void		ft_free_objects(t_object **head);
void		img_destroy(void *mlx, t_img *img);
void		free_obj_list(t_object *object);
void		free_animations(t_world *world, t_display *display);

//graphics.c
t_img		*empty_img(void *mlx, int x, int y);
t_img		*img_from_path(char *image, void *mlx);


//initialization.c
t_display	display_init(int width, int height);			//test
t_img		**frames(char *frames, char *directory, t_display *display);
void		animation_init(t_world *world, t_display *display);
t_world		*world_init(char *map);
t_img		*get_frame(void *t, int c);

//hooks.c
int			handle_keypress(int keycode, t_frame *frame_index);
void		render_ui(t_world *world, t_display *display);
int			render_next_frame(void *param);
int			mouse(int x, int y, void *param);

//main.c
void		print_list(t_object *start);
int			shut(void *param);


//maths.c
int			ran_int(int min, int max);
double		deg_to_rad(double degrees);
t_xy		move_in_dir(t_xy currentPos, double direction, double distance);
t_xy		polar_to_vec2(t_xy start, double angle_rad, int distance);
int			vec2_to_angle(t_xy point1, t_xy point2);


//objects.c
t_object	*new_obj(char *type, void *data);
t_object	*append(t_object **head, t_object *object);
t_enemy		*new_enemy(t_display *display, t_xy pos);
t_coin		*new_coin(t_display *display, t_xy pos);

//parsing.c
int			find_holes(char **array, int rows);
void		count_items(char *array, t_world *world);
char		**read_map(char *file, int rows, t_world *world);
t_tile		**char2tile(t_world *world, int row_count, t_display *display);
char		**scan_map(t_world *world, char *file);

//parsing2.c
void		recur(char **c, t_xy pos, t_world *world, int *exit_found);
char		**clone_char_array(char **c);
int			find_exit(char **c, t_world *world);
int			count_newline(char *filename);

//pixels.c
void		draw_rect(t_img *img, t_xy dimensions, t_xy pos, int color);
void		put_pixel(t_img *data, int x, int y, int color);
void		draw_line(t_img *img, t_xy start, t_xy end, int color);

//positioning.c
int			center(t_img *image, t_img *image2);
t_xy		interpolate(t_xy pos, t_xy pos2, int value);
t_xy		iso_map(t_xy pos);

//printing.c
void		print_2d_tiles(t_tile **c);
void		print_char_array(char **c);

//ray_casting.c
void		ray_init(t_ray *ray, t_xy pos, double angle_deg, int distance);
int			ray_cast(t_world *world, t_xy pos, double angle_deg, int distance);

//rendering_objects.c
t_xy		interpolate(t_xy pos, t_xy pos2, int value);
void		put_pixel(t_img *data, int x, int y, int color);
t_img		*empty_img(void *mlx, int x, int y);
t_img		*img_from_path(char *image, void *mlx);


void		draw_line(t_img *img, t_xy start, t_xy end, int color);
void		draw_rect(t_img *img, t_xy dimensions, t_xy pos, int color);

int			count_newline(char *filename);

void		print_char_array(char **c);
void		print_2d_tiles(t_tile	**c);
void		count_items(char *array, t_world *world);
char		**read_map(char *file, int rows, t_world *world);
t_tile		**char2tile(t_world *world, int row_count, t_display *display);
char		**scan_map(t_world *world, char *file);



void		ray_init(t_ray *ray, t_xy pos, double angle_deg, int distance);
int			ray_cast(t_world *world, t_xy pos, double angle_deg,
				int distance);
char		**clone_char_array(char **c);
int			find_exit(char **c, t_world *world);


int			render_floor(t_world *world, t_display *display, t_img **sprites);
int			render_sentry(t_display *display, t_enemy *enemy);
void		render_world(t_world *world, t_display *display);

void		update_enemies(t_world *world, t_display *display);
int			update_all_animators(t_display *display, t_world *world);



int			draw_fov(t_enemy *enemy, t_display *display,
				t_img *char_array);



int			endgame(t_world *world, t_display *display);
int			mouse(int x, int y, void *param);
int			render_next_frame(void *param);



int			endgame(t_world *world, t_display *display);
void		print_statistics(t_world *world);
void		print_2d_tiles(t_tile **c);
t_xy		render_tile(t_display *display,
				t_img *b_image, t_xy pos, t_xy mod);
int			render_player(t_world *world, t_display *display);
void		print_end_screen(t_world *world);

int img_impose(t_img *canvas, t_img *img, t_xy start, t_xy scaling);
int clear_img(t_img *img);

#	endif
