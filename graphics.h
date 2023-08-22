#ifndef GRAPHICS
# define GRAPHICS

#include <mlx.h>
#include <stdlib.h>
#include <math.h>
#include "libft/libft.h"
#include <stdio.h>
#include <sys/fcntl.h>


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
	int		hp;
	t_animation	*animator;
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
	t_object	*animation;
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

	ret->sprites = malloc(sizeof(t_data *) * 5);
	ret->sprites[0] = put_img("enemy.xpm", ret->mlx);
	ret->sprites[1] = put_img("tile1.xpm", ret->mlx);
	ret->sprites[2] = put_img("tile2.xpm", ret->mlx);

	return(ret);
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

void draw_square(t_data *img, int width, int height, int x_pos, int y_pos)
{
	int x = -1;
	int y;
	
	while(++x < width)
	{
		y = 0;
		while(++y < height)
			put_pixel(img, x+x_pos,y+y_pos, 0x00FF0000);
	}
}

// LINKEDLISt

t_object	*new_object(char *type, void *data)
{
	t_object	*object_list = malloc(sizeof(t_object));
	if (!object_list)
		return(object_list = NULL);
	*object_list = (t_object){type, data, NULL};
	return (object_list);
}

void	object_add_back(t_object **head, t_object *object)
{
	t_object	*current;
	
	if (*head == NULL)
		*head = object;

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
		if (ft_is_charset(buffer, "1P2CES0\n") != 1)
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

int check_objects(char **c)
{
	int	y;
	int exits;
	int player;
	int collectible;
	
	exits = 0;
	player = 0;
	collectible = 0;
	y = -1;
	while(c[++y])
	{
		if(ft_strchr(c[y], 'P') != 0)
			player = 1;
		if(ft_strchr(c[y], 'C') != 0)
			collectible = 1;
		if(ft_strchr(c[y], 'E') != 0)
			exits = 1;
	}
	if(player != 1)
		exit(write(2,"No player\n", 10));
	if(collectible != 1)
		exit(write(2,"No collectible\n", 15));
	if(exits != 1)
		exit(write(2,"No exit\n", 8));
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
	t_object	*object;

	object = world->enemies->next;
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
			if(array[y][x] == 'S' && object)
			{
				row[x].data = object;
				object = object->next;
			}
		}
		row[x] = (t_tile){0, NULL};
		ret_array[y] = row;
	}
	ret_array[y] = NULL;
	return(ret_array);
}

char	**scan_map()
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
	check_objects(array);
	find_holes(array, count);
	print_2d_array(array);
	
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