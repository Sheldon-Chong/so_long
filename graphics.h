#ifndef GRAPHICS_H
# define GRAPHICS_H

# include <mlx.h>
# include <stdlib.h>
# include <math.h>
# include "libft/libft.h"
# include <stdio.h>
# include <sys/fcntl.h>

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
}				t_data;

typedef struct s_animation
{
	int		current_frame;
	int		frame_timer;
	int		speed;
	t_data	**frames;
}	t_animator;

typedef struct s_enemy
{
	t_xy		pos;			
	int			c_ang;			
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
	t_animator	*animator;
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
	t_data		**sprites;
	t_camera	*camera;
	t_object	*animations;
	t_object	*anim_spritesheet;
	t_xy		mouse;
	t_xy		dimensions;
	t_grid_d	*grid_display;
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


int			ran_int(int min, int max);
t_xy		bounce(t_xy pos, t_xy pos2, int value);
t_xy		iso_map(t_xy pos);
t_xy		map_iso(t_xy pos);
void		put_pixel(t_data *data, int x, int y, int color);
t_data		*new_img(void *mlx, int x, int y);
t_data		*put_img(char *image, void *mlx);
t_display	display_init(int width, int height);
t_data		*g_frame(void *t, int c);
t_xy		pos_ang_dis2pos(t_xy start, double angle_rad, int distance);
double		calculate_distance(t_xy point1, t_xy point2);
int			pts2angle(t_xy point1, t_xy point2);
int			center(t_data *image, t_data *image2);
int			create_trgb(int t, int r, int g, int b);
void		draw_line(t_data *img, t_xy start, t_xy end, int color);
void		draw_rect(t_data *img, t_xy dimensions, t_xy pos, int color);
t_object	*new_obj(char *type, void *data);
t_object	*append(t_object **head, t_object *object);
int			count_newline(char *filename);
int			find_holes(char **array, int rows);
void		print_char_array(char **c);
void		print_2d_tiles(t_tile	**c);
void		count_items(char *array, t_world *world);
char		**read_map(char *file, int rows, t_world *world);
t_enemy		*new_enemy(t_display *display, t_xy pos);
t_coin		*new_coin(t_display *display, t_xy pos);
t_tile		**char2tile(t_world *world, int row_count, t_display *display);
char		**scan_map(t_world *world, char *file);
void		ray_init(t_ray *ray, t_xy pos, double angle_deg, int distance);
int			ray_cast(t_world *world, t_xy pos, double angle_deg,
				int distance);
void		animation_init(t_world *world, t_display *display);
t_world		*world_init(char *map);
void		reur(char **c, t_xy pos, t_world *world, int *exit_found);
char		**clone_char_array(char **c);
int			find_exit(char **c, t_world *world);
int			render_floor(t_world *world, t_display *display, t_data **sprites);
int			render_sentry(t_display *display, t_enemy *enemy);
void		render_world(t_world *world, t_display *display);
t_xy		move_in_dir(t_xy currentPos, double direction, double distance);
void		update_enemy(t_world *world, t_display *display);
int			update_animations(t_display *display, t_world *world);
int			enemy_search4player(t_world *world, t_enemy *enemy);
int			decide_enemy_movement(t_world *world, t_enemy *enemy);
void		move_enemy(t_enemy *enemy, t_xy pos, t_world *world);
int			draw_fov(t_enemy *enemy, t_display *display,
				t_data *char_array);
void		ft_free_objects(t_object **head);
int			endgame(t_world *world, t_display *display);
int			mouse(int x, int y, void *param);
int			render_next_frame(void *param);
int			handle_keypress(int keycode, t_frame *current_frame);

int			endgame(t_world *world, t_display *display);
void		print_statistics(t_world *world);
void		print_2d_tiles(t_tile **c);
t_xy		render_tile(t_display *display,
				t_data *b_image, t_xy pos, t_xy mod);
int			render_player(t_world *world, t_display *display);
void		print_end_screen(t_world *world);

#	endif