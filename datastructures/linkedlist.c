#include <stdio.h>
#include <stdlib.h>

typedef struct listnode_t
{
	void *data;
	node* next;
} listnode_t;

listnode_t *list_create(void *data)
{
	listnode_t node;
	if(!(node = malloc(sizeof(listnode_t))))
		return NULL;
	
	node->data = data;
	node->next = NULL;

	return node;
}

listnode_t *insert(listnode_t *list, void *data)
{
	listnode_t *new;
	new = list_create(data);
	new->next = list;

	return new;
}

int remove(listnode_t *list, void *data)
{
	while(list->next && list->data != data)
	{
		list = list->next;
	}
	if(list->next)
	{
		list->next = node->next;
		free(node);
		return 0;
	}
	else return -1;
}
