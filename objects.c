/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 12:03:15 by shechong          #+#    #+#             */
/*   Updated: 2023/09/18 13:48:56 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

t_object	*new_obj(char *type, void *data)
{
	t_object	*object_list;

	object_list = malloc(sizeof(t_object));
	if (!object_list)
		return (NULL);
	object_list->type = type;
	object_list->data = data;
	object_list->next = NULL;
	return (object_list);
}

t_object	*append(t_object **head, t_object *object)
{
	t_object	*current;

	if (*head == NULL)
	{
		*head = object;
		return (object);
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = object;
	return (object);
}

t_enemy	*new_enemy(t_display *display, t_xy pos)
{
	t_enemy	*sentry;
	t_timer	*timer;

	sentry = malloc(sizeof(t_enemy));
	*sentry = (t_enemy){pos, 0, 0, 5,
		(t_animator *)(display->anim_spritesheet->next->data), 0, 0, pos, NULL};
	timer = malloc(sizeof(t_coin));
	*timer = (t_timer){5, 0};
	append(&(sentry->timers), new_obj("timer", timer));
	return (sentry);
}

t_coin	*new_coin(t_display *display, t_xy pos)
{
	t_coin	*collectible;

	collectible = malloc(sizeof(t_coin));
	*collectible = (t_coin){pos,
		(t_animator *)(display->anim_spritesheet->data)};
	return (collectible);
}
