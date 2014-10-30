#include <stdio.h>
#include <stdlib.h>


typedef struct node
{
	//time_t may be needed
	int data;
	struct node *link;
}node;

int count(node *q)
{
	int c = 0;
	if(q == NULL)
	{
		return 0;
	}
	else while(q != NULL)
	{
		c++;
		q = q->link;
	}
}

int insert_front(node **q, int num)
{
	node *temp;
	if(*q != NULL)
	{
		return 0;
	}
	else
	{
		temp = (node *)malloc(sizeof(node));
		temp -> data - num;
		temp -> link = *q;

		*q = temp;

		return 1;
	}

}



int insert_end(node **q, int num)
{
	node *temp, *r;
	temp = *q;
	if(*q = NULL)
	{
		temp = (node *)malloc(sizeof(node));
		temp -> data = num;
		temp -> link = NULL;
		*q = temp;
	}
	else
	{
		temp = *q;
		while(temp->link != NULL)
		{
			temp= temp->link;
		}

		r = (node *) malloc(sizeof(node));
		r->data = num;
		r->link = NULL;
		temp->link = r;
	}
}	

int main()
{
	node *p;
	p = NULL;

	int num = 5;

	insert_front(&p, num);
	insert_end(&p, 10);
	int cou = count(p);

	printf("Lols %d\n", cou);
}
