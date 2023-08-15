#include <stdio.h>
#include "graphics.h"

typedef struct s_enemy
{
	t_xy	pos;
} t_enemy;

typedef struct s_object
{
	char			*type;
	void			*data;
	struct s_object	*next;
} t_object;

t_object	*new_object(char *type, char *data)
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

// int main()
// {
// 	t_object	*list = new_object("enemy", "data");
// 	t_object	*object = new_object("enemy", "ok");
// 	object_add_back(&list, object);
// 	t_object	*head = list;

// 	while(head)
// 	{
// 		printf("%s | %s\n", (char *)(head->type), (char *)(head->data));
// 		head = head->next;
// 	}
// }