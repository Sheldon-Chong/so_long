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
} t_animation;

typedef struct s_enemy
{
	t_xy	pos;
	int		angle;
	int		i_angle;
	int		hp;
	t_animation	*animator;
	int		player_found;
} t_enemy;

typedef struct s_collectible
{
	t_xy	pos;
	t_animation	*animator;
} t_collectible;

typedef struct	s_player 
{
	t_xy	pos;
	t_xy	i_pos;
	t_xy	mapped_pos;
	int		lives;
	t_animation	animator;
}				t_player; 

typedef struct s_graphic_display {
	int width;
	int height;
	void *mlx;
	void 		*mlx_win;
	t_data		*img;
	t_data		**sprites;
	t_camera	*camera;
	t_object	*animations;
	t_object	*anim_spritesheet;
	t_xy		*mouse_pos;
} t_display;



typedef struct s_tile
{
	char type;
	void *data;
} t_tile;

typedef struct s_world {
	t_player	*player;
	t_enemy		*enemy;
	t_object	*enemies;
	t_object	*collectibles;
	char		**grid;
	t_tile		**tgrid;
	long		movement_count;
	t_xy		dimensions;
}	t_world;

typedef struct s_frame {
	t_display	*display;
	t_world				*world;
} t_frame;

typedef struct	frame_data {
	int 		*frame_sec;
	t_world 	*world;
	t_display *graphic_display;
	int 		*i;
} t_frame_data;

// ISOMEtRIC

t_xy	bounce(t_xy pos, t_xy pos2)
{
	return((t_xy){(pos.x + (pos2.x - pos.x)/10), (pos.y + (pos2.y - pos.y)/10)});
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
	img->addr = mlx_get_data_addr(img->img, &(img->bits_per_pixel), &(img->line_length),
								&(img->endian));
	return(img);
}


t_display *graphics_init(int width, int height)
{
	t_camera *camera = malloc(sizeof(t_camera));
	*camera = (t_camera){1,1};
	t_display *ret;

	ret = malloc(sizeof(t_display));

	ret->mlx = mlx_init();
	ret->mlx_win = mlx_new_window(ret->mlx, width, height, "Hello world!");
	ret->width = width;
	ret->height = height;
	ret->camera = camera;
	ret->img = put_img("tile2.xpm", ret->mlx);
	ret->animations = NULL;
	ret->anim_spritesheet = NULL;

	ret->sprites = malloc(sizeof(t_data *) * 5);
	ret->sprites[0] = put_img("enemy.xpm", ret->mlx);
	ret->sprites[1] = put_img("assets/Wall.xpm", ret->mlx);
	ret->sprites[2] = put_img("assets/tile_black.xpm", ret->mlx);
	ret->sprites[3] = put_img("assets/tile_white.xpm", ret->mlx);

	return(ret);
}

// MAtH

double degrees_to_radians(double degrees) {
	return degrees * M_PI / 180.0;
}

t_xy calculate_endpoint(t_xy start, double angle_rad, int distance) {
	return ((t_xy){start.x + (int)(distance * cos(angle_rad)), start.y + (int)(distance * sin(angle_rad))});
}

int	ray_cast(t_world *world, t_xy pos, double angle_deg, int distance, int rows, int cols) {
	double angle_rad = degrees_to_radians(angle_deg);
	t_xy end = calculate_endpoint(pos, angle_rad, distance);
	t_xy difference;
	t_tile **array = world->tgrid;
	int i = 0;

	difference = (t_xy){abs(end.x - pos.x), abs(end.y - pos.y)};

	int step_x = (pos.x < end.x) ? 1 : -1;
	int step_y = (pos.y < end.y) ? 1 : -1;

	int error = difference.x - difference.y;

	// printf("\n");
	// printf("Start [%d, %d]\n", pos.x, pos.y);
	// printf("End [%d, %d]\n", end.x, end.y);
	// printf("Difference_XY [%d, %d]\n", difference.x, difference.y);
	while ((pos.x != end.x || pos.y != end.y) && i < 40) 
	{
		if (pos.x >= 0 && pos.x < cols && pos.y >= 0 && pos.y < rows)
		{
			if(array[pos.y][pos.x].type == '1')
				return 0;
			if(!(array[pos.y][pos.x].type == 'S'))
			   	array[pos.y][pos.x].type = 'H'; 
			if(pos.x == world->player->pos.x && pos.y == world->player->pos.y)
				return 1;
		}
		if (2 * error > -difference.y) //contains the absolute values. Both error and difference are absolute, hence why they have to look at the smallest small, aka -differencne,y since difference is a absolute value.
		{
			error -= difference.y; //corrected to be nearer to the difference
			pos.x += step_x; //incriment by the step, which is determined by where the endpoint is. Can only be neg or positive because 1 bcoz it cannot increase past that
			//printf("1\n");
		}
		if (2 * error < difference.x) 
		{
			error += difference.x;
			pos.y += step_y;
			//printf("2\n");
		}
		i ++;
	}        
	return 0;
}

// COLORING

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

//SHAPES

void draw_line(t_data *img, t_xy start, t_xy end, int color) {
	int dx = end.x - start.x;
	int dy = end.y - start.y;
	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	int i2 = 0;

	float xIncrement = (float)dx / steps;
	float yIncrement = (float)dy / steps;

	float x = start.x;
	float y = start.y;

	for (int i = 0; i <= steps; i++) {
		put_pixel(img, (int)x, (int)y, create_trgb(i2 * 4.4,255,255,255));
		x += xIncrement;
		y += yIncrement;
		i2 ++;
	}
}

void draw_square(t_data *img, int width, int height, int x_pos, int y_pos, int color)
{
	int x = -1;
	int y;
	
	while(++x < width)
	{
		y = -1;
		while(++y < height)
			put_pixel(img, x+x_pos,y+y_pos, color);
	}
}

// LINKEDLISt

t_object	*new_object(char *type, void *data)
{
	t_object	*object_list = malloc(sizeof(t_object));
	if (!object_list)
		return NULL;
	object_list->type = type;
	object_list->data = data;
	object_list->next = NULL;
	return object_list;
}

void	object_add_back(t_object **head, t_object *object)
{
	t_object	*current;
	
	if (*head == NULL)
	{
		*head = object;
		return;
	}
		
	current = *head;
	while(current->next)
		current = current->next;
	current->next = object;
}

int count_newline(char *filename)
{
	char *buffer;	
	int newline_count;
	int status;
	int fd;

	newline_count = 0;
	fd = open("test.ber", 0);
	buffer = malloc(2);
	while ((status = read(fd, buffer, 1)) > 0)
	{
		if (buffer[0] == '\n' || buffer[0] == '\r')
			newline_count++;
		buffer[1] = 0;
		if (ft_is_charset(buffer, "1P2CESH0\n") != 1)
			exit(write(2, "Incorrect characters\n", 19));
	}
	if (status < 0)
		exit(write(2,"Error reading file\n",19));
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

void	print_2d_array(char **c)
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


char	**read_array(char *file, int rows)
{
	int		i;
	char	*buffer;
	char **array;
	int fd;
	
	fd = open(file, 0);
	i = 0;
	buffer = get_next_line(fd);
	array = malloc(sizeof(char *) * (rows + 1));
	while(buffer)
	{
		buffer = ft_substr(buffer, 0, ft_strrchr(buffer, '\n') - buffer);
		array[i++] = buffer;
		buffer = get_next_line(fd);
	}
	array[i] = NULL;
	close(fd);
	return(array);
}

t_tile	**read_array2(t_world *world, int rows)
{
	int x;
	int y;
	t_tile **ret_array;
	t_tile *row;
	char	**array;
	t_object	*sentry_object;
	t_object	*collectible;

	sentry_object = world->enemies;
	collectible = world->collectibles;
	array = world->grid;
	ret_array = malloc(sizeof(t_tile *) * (rows + 1));
	y = -1;
	while(++y < rows)
	{
		row = malloc(sizeof(t_tile) * (ft_strlen(array[0]) + 1));
		x = -1;
		while(++x < ft_strlen(array[y]))
		{
			row[x] = (t_tile){array[y][x], NULL};
			if(array[y][x] == 'S' && sentry_object)
			{
				row[x].data = (t_enemy *)(sentry_object->data);
				sentry_object = sentry_object->next;
			}
			if(array[y][x] == 'C' && collectible)
			{
				row[x].data = collectible;
				collectible = collectible->next;
			}
		}
		row[x] = (t_tile){0, NULL};
		ret_array[y] = row;
	}
	ret_array[y] = NULL;
	return(ret_array);
}

char	**scan_map(t_world *world)
{
	int count;
	char **array;
	char *buffer;
	int i;
	int i2;

	int fd = open("test.ber", 0);
	count = count_newline("test.ber");
	array = malloc(sizeof(char *) * (count + 1));
	array = read_array("test.ber", count);
	i = -1;
	while(array[++i + 1])
	 	if(ft_strlen(array[i]) != ft_strlen(array[i+1]))
			exit(write(2, "Incorrect length\n", 17));
	find_holes(array, count);
	print_2d_array(array);
	world->dimensions.y = count;
	world->dimensions.x = ft_strlen(array[0]);
	
	// i = -1;
	// while(test[++i])
	// {
	// 	i2 = -1;
	// 	while(test[i][++i2].data)
	// 		printf("[%c]", test[i][i2].type);
	// 	printf("\n");
	// }
	return (array);
}


# endif