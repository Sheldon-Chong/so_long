#include <stdio.h>
#include "graphics.h"
#include <fcntl.h>

// void test(char *file, t_data *img, int i2)
// {
// 	int		fd;
// 	int		lines;
// 	int		i;
// 	char	*test;

// 	lines = 0;;
// 	lines = 0;
// 	fd = open(file, 0);
// 	test = get_next_line(fd);
// 	while(test && ft_strncmp(test, "/* pixels */", 11) != 0)
// 	{
// 		test = get_next_line(fd);
// 		lines ++;
// 	}
// 	while(test)
// 	{
// 		test = get_next_line(fd);
// 		test = ft_substr(test, 1, ft_strlen(test) - 4);

// 		if(ft_strchr(test, ';') != 0 || !test)
// 			break;

// 		i = -1;
// 		while(test[++i])
// 		{
// 			if(test[i] != ' ')
// 				draw_line(img, (t_xy){(i * 3) + 250, (lines * 3) + 250}, (t_xy){(i * 3) + 200, (lines * 3) + 200}, 0x00FF0000);
// 		}
// 			//put_pixel(img, 30 + (i * 2), 30 + (lines * 2), 0x00FF0000);
// 		//put_pixel(img, 30 + iso_map((t_xy){lines, i}).x, 30 + iso_map((t_xy){lines, i}).y, 0x00FF0000);

// 		//put_pixel(img, 30 + (lines * 2), 30 + (i * 2), 0x00FF0000);
// 		//printf("%s\n",test);
		
// 		lines ++;
// 	}
	
// }

int handle_keypress(int keycode, t_frame *frame)
{
	t_player *player = frame->world->player;
	t_xy new_pos = player->pos;

	if (keycode == 13 && frame->world->grid[player->pos.y - 1][player->pos.x] != '1') // 'W' key is pressed
		new_pos.y --;
	else if (keycode == 0 && frame->world->grid[player->pos.y][player->pos.x - 1] != '1') // 'A' key is pressed
		new_pos.x --;
	else if (keycode == 1 && frame->world->grid[player->pos.y + 1][player->pos.x] != '1') // 'S' key is pressed
		new_pos.y ++;
	else if (keycode == 2 && frame->world->grid[player->pos.y][player->pos.x + 1] != '1') // 'D' key is pressed
		new_pos.x ++;
	if(new_pos.x > 0 && new_pos.y > 0)
		player->pos = new_pos;
	
	//frame->world->grid[player->pos.y][player->pos.x] = '1';
	return (0);
}


typedef struct s_grid_display {
	float		space_x;
	float		space_y;
	int			offset_x;
	int			offset_y;
} t_grid_display;

typedef struct s_grid {
	int width;
	int height;
	char **grid;
} t_grid;

double degrees_to_radians(double degrees) {
    return degrees * M_PI / 180.0;
}

t_xy calculate_endpoint(t_xy start, double angle_rad, int distance) {
    return ((t_xy){start.x + (int)(distance * cos(angle_rad)), start.y + (int)(distance * sin(angle_rad))});
}

void draw_grid_line(char **array, t_xy pos, double angle_deg, int distance, int rows, int cols) {
    double angle_rad = degrees_to_radians(angle_deg);
    t_xy end = calculate_endpoint(pos, angle_rad, distance);
	t_xy difference;

	difference = (t_xy){abs(end.x - pos.x), abs(end.y - pos.y)};

    int step_x = (pos.x < end.x) ? 1 : -1;
    int step_y = (pos.y < end.y) ? 1 : -1;

    int error = difference.x - difference.y;

    while (pos.x != end.x || pos.y != end.y) 
	{
        if (pos.x >= 0 && pos.x < cols && pos.y >= 0 && pos.y < rows)
		{
			if(array[pos.y][pos.x] == '1')
				return;
			array[pos.y][pos.x] = 'H'; 
		}
        int double_error = 2 * error;

        if (double_error > -difference.y) {
            error -= difference.y;
            pos.x += step_x;
        }
        if (double_error < difference.x) {
            error += difference.x;
            pos.y += step_y;
        }
    }
    if (end.x >= 0 && end.x < cols && end.y >= 0 && end.y < rows)
        array[end.y][end.x] = 'H';
}


int render_player(t_world *world, t_xy pos, t_graphic_display *display)
{
	void	*mlx	 = display->mlx;
	void	*mlx_win = display->mlx_win;
	t_player *player = world->player;
	char	**grid = world->grid;

	void	*img = put_img("./p1.xpm", mlx)->img;

	mlx_put_image_to_window(mlx, mlx_win, img, pos.x, pos.y);	
	free(img);
	return 1;
}

int render_grid(t_world *world, t_graphic_display *display, char **c, t_grid_display grid, t_data **sprites)
{
	int		tile_x = -1;
	int		tile_y = 0;
	void	*mlx = display->mlx;
	void	*mlx_win = display->mlx_win;
	t_data	*image;

	while(++tile_x < 10)
	{
		tile_y = -1;
		while(++tile_y < 10)
		{
			image = sprites[2]->img;
			if(c[tile_y][tile_x] == '1')
				image = sprites[1]->img;
			if(c[tile_y][tile_x] != 'H')
				mlx_put_image_to_window(mlx, mlx_win, image, 
					iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).x + grid.offset_x,
					iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).y + grid.offset_y);
			if(tile_x == world->player->pos.x && tile_y == world->player->pos.y)
			{
				world->player->i_pos = iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y});
				render_player(world, (t_xy){world->player->i_pos.x + grid.offset_x + 12, world->player->i_pos.y + grid.offset_y - 25}, display);
			}
			if(c[tile_y][tile_x] == 'S')
			{
				mlx_put_image_to_window(mlx, mlx_win, sprites[1]->img, 
				iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).x + grid.offset_x,
				iso_map((t_xy){tile_x * grid.space_x, tile_y * grid.space_y}).y + grid.offset_y);
			}
		}
	}
	return (1);
}

t_xy	bounce(t_xy pos, t_xy pos2)
{
	return((t_xy){(pos.x + (pos2.x - pos.x)/10), 
				(pos.y + (pos2.y - pos.y)/10)});
}

int render_next_frame(void *param)
{
	t_frame_data *data = (t_frame_data *)param;
	void *mlx = data->graphic_display->mlx;
	void *mlx_win = data->graphic_display->mlx_win;
	t_world *world = data->world;
	t_graphic_display *display = data->graphic_display;
	int mouse_x, mouse_y;
	int i = *(data->i);
	
	(*(data->frame_sec))++;
	if (*(data->frame_sec) > 100)
	{
		mlx_clear_window(mlx, mlx_win);
		*(data->frame_sec) = 0;
		display->camera->pos = bounce(display->camera->pos, world->player->i_pos);
		//printf("[%d, %d] [%d, %d,]\n", display->camera->pos.x, display->camera->pos.y, world->player->i_pos.x, world->player->i_pos.y);
		render_grid(world, display, world->grid, (t_grid_display){21, 21, ((display->width) / 2) - display->camera->pos.x, ((display->height) / 2) - display->camera->pos.y}, data->graphic_display->sprites);
		draw_grid_line(world->grid, (t_xy){1,1}, i, 20,10,10);
		mlx_string_put(mlx,mlx_win, 10, 10, 0x00FF0000, "test");
		(*(data->i))++;
	}
	return (1);
}

typedef struct t_counter{
	int player;
	int sentry;
	int collectible;
	int wall;
	int empty;
	int hole;
	int exit;
} t_counter;

void get_objects(char **array)
{
	int x;
	int y;

	t_counter *count = malloc(sizeof(t_counter));
	*count = (t_counter){0,0,0,0,0,0,0};
	t_object	*sentry_list = new_object("null", "data");
	t_object	*object;
	y = -1;
	while(array[++y])
	{
		x = -1;
		while(array[++x])
		{
			if (array[y][x] == 'C')
				count->collectible ++;
			if (array[y][x] == '0') 
				count->empty ++;
			if (array[y][x] == '1') 
				count->wall ++;
			if (array[y][x] == 'E') 
				count->exit ++;
			if (array[y][x] == 'S')
			{
				count->sentry ++;
				t_enemy *sentry = malloc(sizeof(t_enemy));
				*sentry = (t_enemy){x,y};
				object_add_back(&sentry_list, new_object("enemy", sentry));
			}
			if (array[y][x] == 'P')
				count->player ++;
			if (array[y][x] == 'H')
				count->hole ++;
		}
	}
	printf("player: %d\n",count->player);
	printf("sentry: %d\n",count->sentry);
	printf("coin: %d\n",count->collectible);
	printf("wall: %d\n",count->wall);
	printf("empty: %d\n",count->empty);
	printf("hole: %d\n",count->hole);
	printf("exit: %d\n",count->exit);

	t_object *head = sentry_list;
	while(head)
	{
		printf("%s | %s | [pos: %d, %d]\n", head->type, head->data, ((t_enemy *)(head->data))->pos.x, ((t_enemy *)(head->data))->pos.y);
		head = head->next;
	}
}

int calculate_image_width(t_data *image) {
    if (image->bits_per_pixel != 0) {
        return image->line_length / (image->bits_per_pixel / 8);
    } else {
        // Handle the case where bits_per_pixel is 0
        // You can return an error code or take appropriate action
        // For example, you can return -1 to indicate an error.
        return -1;
    }
}

void draw_ray(char **array, int start_x, int start_y, double angle_deg, int rows, int cols) {
    double angle_rad = angle_deg * M_PI / 180.0; // Convert angle to radians

    int dx = (angle_rad >= 0.0 && angle_rad <= M_PI) ? 1 : -1;
    int dy = (angle_rad >= M_PI_2 && angle_rad <= 3 * M_PI_2) ? -1 : 1;

    double tan_angle = tan(angle_rad);
    double inv_tan_angle = 1.0 / tan_angle;

    int current_x = start_x;
    int current_y = start_y;

    while (current_x >= 0 && current_x < cols && current_y >= 0 && current_y < rows) {
        if (array[current_y][current_x] == '1') {
            break; // Hit a '1', stop the ray
        }

        array[current_y][current_x] = 'H'; // Set the cell to 'H'

        // Calculate the next step based on the angle quadrant
        int next_x = current_x + dx;
        int next_y = current_y + dy;

        double delta_x = fabs(1.0 / tan_angle);
		double delta_y = fabs(tan_angle);

        if (dx == -1) {
            delta_x = -delta_x;
        }
        if (dy == -1) {
            delta_y = -delta_y;
        }

        // Check if the ray is closer to the next horizontal or vertical cell
        if (delta_x < delta_y) {
            current_x = next_x;
        } else {
            current_y = next_y;
        }
    }
}
int	main(void)
{
	t_graphic_display	*display = graphics_init(1920, 1080);
	t_player 			*player = malloc(sizeof(t_player));
	t_world 			*world = malloc(sizeof(world));

	player->pos = (t_xy){1, 1};
	player->i_pos = (t_xy){1, 1};
	player->lives = 4;

	char c[10][10] = {
	"1011111111",
	"1000000001",
	"1000000001",
	"100000P001",
	"1000001111",
	"100E000101",
	"1000000101",
	"1000000101",
	"1000000001",
	"1001111111"
	};

	char **test = malloc((11 * sizeof(char*)));

	int i = -1;
	while(++i < 10)
	{
		test[i] = malloc(sizeof(char *) + 1);
		ft_strlcpy(test[i], c[i], 11);
	}
	test[i] = NULL;

	get_objects(test);

	t_enemy	*enemy;
	
	world = &((t_world){player, enemy, test});
	
	t_frame vars;
	vars.world = world;
	vars.display = display;

	int		sec = 0;
	int		frame_sec = 0;

	t_xy my_pos = {0,0};
	t_xy vector = {1,2};

	int i2 = -1;
	while(++i2 < 5)
	{
		
		//draw_ray(test, 1,1,26,10,10);
		if(my_pos.y < 5 && my_pos.x < 5)
			test[my_pos.y][my_pos.x] = '1';
	}

	t_frame_data frame = (t_frame_data){&frame_sec, world, display, &i};
	mlx_hook(display->mlx_win, 2, 1L << 0, handle_keypress, &vars);
	mlx_loop_hook(display->mlx, render_next_frame, &frame);
	mlx_loop(display->mlx);
}