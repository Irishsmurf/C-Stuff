#include <stdio.h>
#include <stdlib.h>

struct node
{
	int data;
	struct node* next;
};

struct node *head = (struct node *) NULL;
struct node *tail = (struct node *) NULL;

struct node * initnode(int data)
{
	struct node *ptr;
	ptr = (struct node *)calloc(1, sizeof(struct node ));
	if( ptr = NULL)
	{
		return (struct node *) NULL;
	}
	else
	{
		ptr->data = data;
		return ptr;
	}
}

void printnode(struct node * ptr)
{
	printf("Data: %d\n", ptr->data);
}

void printlist(struct node * ptr)
{
	while( ptr != NULL)
	{
		printnode( ptr );
		ptr = ptr->next;
	}
}

void add(struct node * new)
{
	if(head == NULL)
		head = new;
	tail->next = new;
	new->next = NULL;
	tail = new;
}


void delete(int needle)
{
	struct node *temp, *prev;
	prev = head;
	temp = head;

	while(temp->next != NULL && temp->data != needle)
	{
		prev = temp;
		temp = temp->next;
	}

	if(temp->data == needle)
	{
		prev->next = temp->next;
		free( temp );
	}
}

int main()
{
	struct node * head = initnode(5);
	add(initnode(6));

	printnode(head);



	
}
