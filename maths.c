/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 11:29:36 by shechong          #+#    #+#             */
/*   Updated: 2023/09/14 12:03:46 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

int	ran_int(int min, int max)
{
	return (rand() % (max - min + 1) + min);
}

t_xy	pos_ang_dis2pos(t_xy start, double angle_rad, int distance)
{
	return ((t_xy){start.x + (int)(distance * cos(angle_rad)),
		start.y + (int)(distance * sin(angle_rad))});
}

double	calculate_distance(t_xy point1, t_xy point2)
{
	int	dx;
	int	dy;

	dx = point2.x - point1.x;
	dy = point2.y - point1.y;
	return (sqrt(dx * dx + dy * dy));
}

int	pts2angle(t_xy point1, t_xy point2)
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
	return ((int)(angle_deg + 0.5));
}

t_xy	move_in_dir(t_xy currentPos, double direction, double distance)
{
	t_xy	new_pos;
	double	rad_dir;

	rad_dir = direction * M_PI / 180.0;
	new_pos.x = currentPos.x + distance * cos(rad_dir);
	new_pos.y = currentPos.y + distance * sin(rad_dir);
	return (new_pos);
}
