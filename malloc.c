#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_node {
    void            *data;
    char            *text;
    struct s_node   *next;
} t_node;

void *gallog(t_node **head, int space)
{
    t_node  *current = *head;
    t_node  *new;
    void    *ret;

    ret = malloc(space);
    new = malloc(sizeof(t_node));
    new->data = ret;
    new->text = "test";
    new->next = NULL;  // Initialize the 'next' pointer of the new node

    if (*head == NULL)
        *head = new;
    else
    {
        while (current->next)
            current = current->next;
        current->next = new;
    }
    
    return ret;
}

int main()
{
    t_node *list = NULL;

    char *test = (char *)gallog(&list, 100 * sizeof(char));
    strcpy(test, "hello");  // Use strcpy to copy strings
    test = (char *)gallog(&list, 100 * sizeof(char));  // Allocate new memory for 'test'
    strcpy(test, "lol");

    printf("%s\n", list->text);

    // Free allocated memory
    t_node *current = list;
    while (current)
    {
        t_node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
	char *meme = malloc(100);
	system("leaks a.out");
    return 0;
}