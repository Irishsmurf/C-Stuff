t
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define ETIMEDOUT 110

struct room
{
	time_t time;
	int room;
};

typedef struct room room;


//---------Linked List Starts Here
struct node
{
	room data;
	struct node* next;
};

typedef struct node node;

int add(node* head, room data)
{
	node* temp = (node *)malloc(sizeof(node));	
	
	//New List
	temp->data = data;
	temp->next = head->next;
	head->next = temp;

	return 0;
}

int length(struct node* list)
{
	int count = 0;
	if(list == NULL)
	{
		return 0;
	}
	else
	{
		while(list->next != NULL)
		{
			printf("One Interation\t\n");			
			count++;
			list = list->next;
		}
	}

	return count;
}


room search(struct node* head)
{

	//Search for lowest and remove it then return it.
	node* iter = head->next;
	room low;
	time_t lowest = iter->data.time;
	low = iter->data;

	while(iter != NULL)
	{
		if(lowest > iter->data.time)
		{
			lowest = iter->data.time;
			low = iter->data;
		}
		iter = iter->next;
	}

	return low;
}


//----------Linked List Ends Here



struct room soonest;
int expired = 0;
int waiting = 0;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


static void* producer(void *arg)
{
	struct tm * date;
	char buffer [80];
	node* head = (node *) arg;
	while(1)
	{
		//Seed Random Number Gen.
		srand(time( NULL ));
		//Get random sleep time
		unsigned int timeout = random() % 5 + 1;
		sleep(timeout);
		
		//Random Room + time
		int room = random() % 9999;
		time_t alarm = time( NULL ) + random() % 60;

		//Covert the argument back to the shared linked list.
		struct node* head = (struct node*) arg;
		
		//Create room
		room *place;
		place = (room*)malloc(sizeof(room));

		place->room = room;
		place->time = alarm;
		
		date = gmtime(&alarm);
		if( waiting == 0)
		{
			soonest.time = alarm;
			soonest.room = room;
			waiting++;
			printf("Registering:\t%4d %s\n", room, ctime(&alarm));
		}
		else
		{
			if(soonest.time < alarm)
			{
				add(head, *place);
				waiting++;
				printf("Registering:\t%4d %s\n", room, ctime(&alarm));
			}
			else
			{
				add(head, soonest);
				soonest = *place;
				waiting++;
				printf("Registering:\t%4d %s\n", room, ctime(&alarm));
				pthread_cond_signal(&cond);
			}
		
		}

	
	}
}

static void* consumer(void *arg)
{
	int rc;
	struct timespec ts;
	node* head = (node*) arg;
	while(1)
	{
		while(waiting != 0)
		{
			ts.tv_sec = soonest.time;
			ts.tv_nsec = 0;
			int room = soonest.room;
			//times = gmtime(&soonest);
			rc = pthread_cond_timedwait(&cond, &mutex, &ts);
			if(rc == ETIMEDOUT)
			{
				printf("Wake Up:\t%4d %s\n", soonest.room, ctime(&ts.tv_sec));
				expired++;
				waiting--;
				printf("Expired Alarms:\t%d\nPending Alarms:\t%d\n\n", expired,waiting);
				rc = 2;
				//soonest = search(head);
				soonest.time = 99999999999;
			}
			else if(rc == 0)//New Soonest added, store soonest back in list
			{
				room *rm;
				rm->room = room;
				rm->time = ts.tv_sec;
				add(head, *rm);
				
			}

		}
	}

}


int main()
{

	pthread_t alarmCreator;
	pthread_t alarmPrinter;

	printf("Lol\n");
	struct node* nodes = NULL;
	pthread_create(&alarmCreator, NULL, producer, (void *) nodes);
	pthread_create(&alarmPrinter, NULL, consumer, (void *) nodes);
	
	pthread_join(alarmCreator, NULL);

/*
	room *rm;
	node *nodes;
	node *temp;
	rm = (room *)malloc(sizeof(room));
	nodes = (node *)malloc(sizeof(node));

	rm->time = time( NULL );
	rm->room = 5;

	

	add(nodes, *rm);
	add(nodes, *rm);
	add(nodes, *rm);
	add(nodes, *rm);
	add(nodes, *rm);
	room rum = search(nodes);
	printf("LOWEST = %s\n", ctime(&rum.time));
	printf("Length = %d\n", length(nodes));

*/
	return 0;
}
