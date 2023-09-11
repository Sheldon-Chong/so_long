#ifndef GRAPHICS
# define GRAPHICS

#include <mlx.h>
#include <stdlib.h>
#include <math.h>
#include "libft/libft.h"
#include <stdio.h>
#include <sys/fcntl.h>

int generateRandomInt(int min, int max) {
	return rand() % (max - min + 1) + min;
}

typedef struct	s_xy {
	int		x;
	int		y;
}				t_xy;

typedef struct s_timer {
	int stopper;
	int elapsed;
}				t_timer;

typedef struct s_object
{
	char			*type;
	void			*data;
	struct s_object	*next;
} t_object;

typedef struct camera {
	t_xy	pos;
	t_xy	pos_goto;
} t_camera;

typedef struct	s_data {
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
} t_animator;

typedef struct s_enemy
{
	t_xy	pos;			//Position
	int		c_ang;			//Current angle
	int		final_angle;		//Angle to turn towards
	int		hp;				//Health
	t_animator	*animator;	//Animator
	int		player_found;	//Bool for whether it has the player in its sights
	int		alert;			//A value that is set to 100 when player is detected. As long as it is over 0, the enemy will look towards the player. It will constantly deduct
	t_xy	current_pos;
	t_object	*timers;
} t_enemy;

typedef struct s_collectible
{
	t_xy	pos;
	t_animator	*animator;
} t_coin;

typedef struct	s_player 
{
	t_xy	pos;
	t_xy	mapped_pos;
	t_animator	animator;
}				t_player; 

typedef struct s_display {
	int width;
	int height;
	void *mlx;
	void 		*mlx_win;
	t_data		*img;
	t_data		**sprites;
	t_camera	*camera;
	t_object	*animations;
	t_object	*anim_spritesheet;
	t_xy		mouse_pos;
	t_xy		dimensions;
} t_display;

typedef struct t_counter{
	int player;
	int sentry;
	int collectible;
	int moves;
	int exit;
} t_counter;

typedef struct s_tile
{
	char type;
	void *data;
} t_tile;

typedef struct s_world {
	t_player	*player;
	t_object	*enemies;
	t_object	*coins;
	char		**grid;
	t_tile		**tgrid;
	t_xy		dimensions;
	t_counter		count;
}	t_world;

typedef struct	frame_data {
	int 		*frame_sec;
	t_world 	*world;
	t_display 	*display;
} t_frame;

// ISOMEtRIC

t_xy	bounce(t_xy pos, t_xy pos2, int value)
{
	return((t_xy){(pos.x + (pos2.x - pos.x)/value), (pos.y + (pos2.y - pos.y)/value)});
}


t_xy iso_map(t_xy pos)
{
	return (t_xy){(pos.x - pos.y) * 1.4, (pos.x + pos.y) / 1.4};
}


t_xy map_iso(t_xy pos) {
	float tempX = (pos.x * 0.7 + pos.y * 0.7) / 2.0;
	float tempY = (pos.y * 0.7 - pos.x * 0.7) / 2.0;
	return (t_xy){tempX, tempY};
}

// RENDERING
void	put_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

t_data	*new_img(void *mlx, int x, int y)
{
	t_data	*img;

	img = malloc(sizeof(t_data));
	img->img = mlx_new_image(mlx, x, y);
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
	if (img->img == NULL) 
	{
		printf("%s\n", image);
		free(img);
		return NULL;
	}
	img->addr = mlx_get_data_addr(img->img, &(img->bits_per_pixel), &(img->line_length),
								&(img->endian));
	return(img);
}


t_display *display_init(int width, int height)
{
	t_camera *camera = malloc(sizeof(t_camera));
	*camera = (t_camera){(t_xy){1,1}, (t_xy){1,1}};
	t_display *ret;

	ret = malloc(sizeof(t_display));
	ret->dimensions = (t_xy){width, height};
	ret->mlx = mlx_init();
	ret->mlx_win = mlx_new_window(ret->mlx, width, height, "Hello world!");
	ret->width = width;
	ret->height = height;
	ret->camera = camera;
	ret->animations = NULL;
	ret->anim_spritesheet = NULL;
	ret->mouse_pos = (t_xy){0,0};

	ret->sprites = malloc(sizeof(t_data *) * 5);
	ret->sprites[0] = put_img("enemy.xpm", ret->mlx);
	ret->sprites[1] = put_img("assets/Wall.xpm", ret->mlx);
	ret->sprites[2] = put_img("assets/tile_black.xpm", ret->mlx);
	ret->sprites[3] = put_img("assets/tile_white.xpm", ret->mlx);
	ret->sprites[4] = put_img("assets/Exit.xpm", ret->mlx);

	return(ret);
}

t_data *g_frame(void *t, int c)
{
	if(c == 0)
		return (((t_coin*)t)->animator->frames[((t_coin*)t)->animator->current_frame]);
	if(c == 1)
		return (((t_enemy*)t)->animator->frames[((t_enemy*)t)->animator->current_frame]);
	return(NULL);
}

// MAtH

double deg2rad(double degrees) {
	return degrees * M_PI / 180.0;
}

t_xy pos_ang_dis2pos(t_xy start, double angle_rad, int distance) {
	return ((t_xy){start.x + (int)(distance * cos(angle_rad)), start.y + (int)(distance * sin(angle_rad))});
}

double calculate_distance(t_xy point1, t_xy point2) 
{
	int	dx;
	int	dy;

	dx = point2.x - point1.x;
	dy = point2.y - point1.y;
	return (sqrt(dx * dx + dy * dy));
}

int pts2angle(t_xy point1, t_xy point2) 
{
	double	dx;
	double	dy;
	double	angle_rad;
	double	angle_deg;

	dx = (double)(point2.x - point1.x);
	dy = (double)(point2.y - point1.y);
	angle_rad = atan2(dy, dx);
	angle_deg = angle_rad * (180.0 / M_PI);

	if (angle_deg < 0)
		angle_deg += 360.0;
	return (int)(angle_deg + 0.5);
}

typedef struct s_ray
{
	double	angle_rad;
	t_xy	end;
	t_xy	dif;
	int		step_x;
	int		step_y;
	int		error;
}	t_ray;

void ray_init(t_ray *ray, t_xy pos, double angle_deg, int distance)
{
	ray->angle_rad = deg2rad(angle_deg);
	ray->end = pos_ang_dis2pos(pos, ray->angle_rad, distance);
	ray->dif = (t_xy){abs(ray->end.x - pos.x), abs(ray->end.y - pos.y)};
	ray->step_x = -1 + ((pos.x < ray->end.x) * 2);
	ray->step_y = -1 + ((pos.y < ray->end.y) * 2);
	ray->error =  ray->dif.x - ray->dif.y;
}

int	ray_cast(t_world *world, t_xy pos, double angle_deg, int distance, int rows, int cols) 
{
	t_ray	ray;
	
	ray_init(&ray, pos, angle_deg, distance);
	while ((pos.x != ray.end.x || pos.y != ray.end.y)) 
	{
		if (pos.x >= 0 && pos.x < cols && pos.y >= 0 && pos.y < rows)
		{
			if(world->tgrid[pos.y][pos.x].type == '1')
				return 0;
			if(pos.x == world->player->pos.x && pos.y == world->player->pos.y)
				return 1;
		}
		if (2 * ray.error > -ray.dif.y)
		{
			ray.error -= ray.dif.y;
			pos.x += ray.step_x;
		}
		if (2 * ray.error < ray.dif.x) 
		{
			ray.error += ray.dif.x;
			pos.y += ray.step_y;
		}
	}
	return	(0);
}

int	center(t_data *image, t_data *image2)
{
	return(((image->line_length)/4 - (image2->line_length) / 4) / 2);
}

// COLORING

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

//SHAPES

void draw_line(t_data *img, t_xy start, t_xy end, int color) 
{
	int		steps;
	int		i2;
	float	x;
	float	y;

	steps = abs(end.y - start.y);
	if (abs(end.x - start.x) > abs(end.y - start.y))
		steps = abs(end.x - start.x);
	x = start.x;
	y = start.y;
	i2 = -1;
	while (++i2 <= steps)
	{
		put_pixel(img, (int)x, (int)y, color);
		x += (float)(end.x - start.x) / steps;
		y += (float)(end.y - start.y) / steps;
	}
}

void draw_square(t_data *img, int width, int height, int x_pos, int y_pos, int color)
{
	int	x;
	int	y;
	
	x = - 1;
	while(++x < width)
	{
		y = -1;
		while(++y < height)
			put_pixel(img, x+x_pos,y+y_pos, color);
	}
}

// LINKEDLISt

t_object	*new_obj(char *type, void *data)
{
	t_object	*object_list = malloc(sizeof(t_object));
	if (!object_list)
		return NULL;
	object_list->type = type;
	object_list->data = data;
	object_list->next = NULL;
	return object_list;
}

t_object	*append(t_object **head, t_object *object)
{
	t_object	*current;
	
	if (*head == NULL)
	{
		*head = object;
		return object;
	}
	current = *head;
	while(current->next)
		current = current->next;
	current->next = object;
	return(object);
}

int count_newline(char *filename)
{
	char	*buffer;	
	int		newline_count;
	int		status;
	int		fd;

	newline_count = 0;
	fd = open(filename, 0);
	buffer = malloc(2);
	while ((status = read(fd, buffer, 1)) > 0)
	{
		if (buffer[0] == '\n' || buffer[0] == '\r')
			newline_count++;
		buffer[1] = 0;
		if (ft_is_charset(buffer, "1P2CESH0\n") != 1)
			exit(write(2, "Error: Incorrect characters\n", 19));
	}
	if (status < 0)
		exit(write(2,"Error: reading file\n",20));
	return (newline_count + 1);
}

int find_holes(char **array, int rows)
{
	int	i;

	i = -1;
	if(!array || !array[0])
		return(0);
	while(array[0][++i])
		if(array[0][i] != '1' || array[rows - 1][i] != '1')
			exit(write(2, "Not fully walled\n", 16));
	i = -1;
	while(array[++i])
		if(array[i][0] != '1' || array[i][ft_strlen(array[0]) - 1] != '1')
			exit(write(2, "Not fully walled\n", 16));
	return(0);
}

void	print_char_array(char **c)
{
	int	y;
	int	x;

	y = -1;
	while(c[++y])
	{
		x = -1;
		while(c[y][++x])
			ft_putchar_fd(c[y][x], 1);
		ft_putchar_fd('\n', 1);
	}
}

void	print_2d_tiles(t_tile	**c)
{
	int	y;
	int	x;

	y = 0;
	x = -1;
	ft_putstr_fd("\n     ",1);
	while(c[0][++x].type)
	{
		ft_putchar_fd("123456789|"[y], 1);
		y = (y + 1) % 10;
	}
	ft_putstr_fd("\n\n",1);
	y = -1;
	while(c[++y])
	{
		x = -1;
		ft_putnbr_fd(y,1);
		if(y < 10)
			ft_putstr_fd(" ",1);
		ft_putstr_fd(" | ",1);
		while(c[y][++x].type)
			ft_putchar_fd(c[y][x].type, 1);
		ft_putchar_fd('\n', 1);
	}
}

typedef struct s_count
{
	int	player_count;
	int	coins;
	int coins_collected;
	int steps_moved;

}	t_count;

t_count	*count_items(char *array, t_world *world)
{
	int i;

	i = -1;
	while(array[++i])
	{
		if(array[i] == 'P')
			world->count.player ++;
		if(array[i] == 'C')
			world->count.collectible ++;
		if(array[i] == 'E')
			world->count.exit ++;
	}
	return(NULL);
}


char	**read_map(char *file, int rows, t_world *world)
{
	int		i;
	char	*buffer;
	char	**array;
	int		fd;
	
	fd = open(file, 0);
	i = 0;
	buffer = get_next_line(fd);
	array = malloc(sizeof(char *) * (rows + 1));
	while(buffer)
	{
		array[i++] = ft_substr(buffer, 0, ft_strrchr(buffer, '\n') - buffer);
		count_items(array[i - 1], world);
		free(buffer);
		buffer = get_next_line(fd);
	}
	array[i] = NULL;
	close(fd);
	if(world->count.player < 1)
		exit(write(1, "Error: Incorrect number of players\n", 34));
	if(world->count.exit < 1)
		exit(write(1, "Error: No exit\n", 15));
	if(world->count.collectible < 1)
		exit(write(1, "Error: No collectible\n", 21));
	return (array);
}

t_enemy	*new_enemy(t_display *display, t_xy pos)
{
	t_enemy *sentry = malloc(sizeof(t_enemy));
	*sentry = (t_enemy){pos, 0, 0, 5, (t_animator *)(display->anim_spritesheet->next->data), 0, 0, pos, NULL};

	t_timer *timer = malloc(sizeof(t_coin));
	*timer = (t_timer){5,0};
	append(&(sentry->timers), new_obj("timer", timer));
	return(sentry);
}

t_coin *new_coin(t_display *display, t_xy pos)
{
	t_coin	*collectible = malloc(sizeof(t_coin));
	*collectible = (t_coin){pos, (t_animator *)(display->anim_spritesheet->data)};

	return(collectible);
}

t_tile	**char2tile(t_world *world, int row_count, t_display *display)
{
	int		x;
	int		y;
	t_tile	**ret_array;

	ret_array = malloc(sizeof(t_tile *) * (row_count + 1));
	y = -1;
	while (++y < row_count)
	{
		ret_array[y] = malloc(sizeof(t_tile) * (ft_strlen(world->grid[0]) + 1));
		x = -1;
		while (++x < (int)(ft_strlen(world->grid[y])))
		{
			ret_array[y][x] = (t_tile){world->grid[y][x], NULL};
			if (world->grid[y][x] == 'S')
				ret_array[y][x].data = ((t_enemy *)(append(&world->enemies, \
		new_obj("enemy", new_enemy(display, (t_xy){x, y}))))->data);
			if (world->grid[y][x] == 'C')
				ret_array[y][x].data = (t_coin *)(append(&world->coins, \
				new_obj("c", new_coin(display, (t_xy){x, y})))->data);
			if (world->grid[y][x] == 'P')
				world->player->pos = (t_xy){x,y};
		}
		ret_array[y][x] = (t_tile){0, NULL};
	}
	ret_array[y] = NULL;
	return (ret_array);
}

char	**scan_map(t_world *world, char *file)
{
	int line_count;
	char **char_array;
	int i;
	int fd;

	fd = open(file, 0);
	if(fd == -1)
		exit(write(2, "Error: Cannot read file\n", 23));
	line_count = count_newline(file);
	if(line_count == 1)
		exit(write(2, "Error: Incorrect length\n", 23));
	char_array = malloc(sizeof(char *) * (line_count + 1));
	char_array = read_map(file, line_count, world);
	i = -1;
	while(char_array[++i + 1])
	 	if(ft_strlen(char_array[i]) != ft_strlen(char_array[i+1]))
			exit(write(2, "Error: Incorrect length\n", 23));
	find_holes(char_array, line_count);
	print_char_array(char_array);
	world->dimensions.y = line_count;
	world->dimensions.x = ft_strlen(char_array[0]);
	if(world->dimensions.y == world->dimensions.x)
		exit(write(2, "Error: Incorrect length\n", 23));
	print_char_array(char_array);
	return (char_array);
}

# endif