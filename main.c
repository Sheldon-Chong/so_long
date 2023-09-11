#include <stdio.h>
#include "graphics.h"
#include <fcntl.h>

void	ft_free_objects(t_object **head)
{
	t_object	*current;
	t_object	*next;

	current = *head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

int	endgame(t_world *world, t_display *display)
{
	t_object	*current;
	t_object	*next;

	mlx_destroy_window(display->mlx, display->mlx_win);

	current = world->enemies;
	while (current)
	{
		next = current->next;
		free(((t_enemy *)(current->data))->timers);
		free(((t_enemy *)(current->data))->timers->data);
		free(current);
		current = next;
	}
	current = world->coins;
	while (current)
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}

	current = display->animations;
	while (current)
	{
		next = current->next;
		printf("<%p>\n", current->data);
		ft_free_array((void *)(((t_animator *)(current->data))->frames), 0);
		current = next;
	}

	current = display->anim_spritesheet;
	while (current)
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}

	int i = 0;
	while(world->tgrid[i])
	{
		free(world->tgrid[i]);
		i ++;
	}

	i = 0;
	while(world->grid[i])
	{
		free(world->grid[i]);
		i ++;
	}
	free(display->camera);

	printf("%lu t_animator\n", sizeof(t_animator));
	printf("%lu t_camera\n", sizeof(t_camera));
	printf("%lu t_coin\n", sizeof(t_coin));
	printf("%lu t_counter\n", sizeof(t_counter));
	printf("%lu t_data\n", sizeof(t_data));
	printf("%lu t_display\n", sizeof(t_display));
	printf("%lu t_enemy\n", sizeof(t_enemy));
	printf("%lu t_frame\n", sizeof(t_frame));

	printf("%lu t_list\n", sizeof(t_list));
	printf("%lu t_object\n", sizeof(t_object));
	printf("%lu t_player\n", sizeof(t_player));
	printf("%lu t_ray\n", sizeof(t_ray));
	printf("%lu t_tile\n", sizeof(t_tile));
	printf("%lu t_timer\n", sizeof(t_timer));
	printf("%lu t_world\n", sizeof(t_world));
	printf("%lu t_xy\n", sizeof(t_xy));

	system("leaks program");
	exit(0);
}

int	main(void)
{
	t_display	*display;
	t_world		*world;
	t_frame		frame;
	int			frame_sec;

	display = display_init(1920, 1080);
	world = world_init("map.ber");
	animation_init(world, display);
	world->coins = NULL;
	world->enemies = NULL;
	world->tgrid = char2tile(world, count_newline("map.ber"), display);
	if (find_exit(world->grid, world) == 0)
		exit(write(2, "Error: Player cannot reach exit\n", 32));
	print_statistics(world);
	frame_sec = 0;
	frame = (t_frame){&frame_sec, world, display};
	mlx_hook(display->mlx_win, 2, 1L << 0, handle_keypress, &frame);
	mlx_loop_hook(display->mlx, render_next_frame, &frame);
	mlx_hook(display->mlx_win, 6, 1L << 6, mouse, &frame);
	mlx_loop(display->mlx);
}
