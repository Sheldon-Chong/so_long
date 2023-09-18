/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   positioning.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:55:44 by shechong          #+#    #+#             */
/*   Updated: 2023/09/11 18:55:49 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

int	center(t_data *image, t_data *image2)
{
	return (((image->line_length) / 4 - (image2->line_length) / 4) / 2);
}

t_xy	bounce(t_xy pos, t_xy pos2, int value)
{
	return ((t_xy){(pos.x + (pos2.x - pos.x) / value),
		(pos.y + (pos2.y - pos.y) / value)});
}
