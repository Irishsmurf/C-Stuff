/*

Operating Systems - CA321

David Kernan	-	59597883
Niall Mohan		-	59375538

*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define ETIMEDOUT 110

// Room Structure
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
//Return Length of Linked List
int length(struct node* list)
{
	int count = 0;
	if(list == NULL)
	{
		return 0;
	}
	else
	{
		//While the next is not empty
		while(list->next != NULL)
		{
			//Keep going
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
	node* prev0 = head;
	node* prev = head;
	node* del = head->next;
	room low; //Lowest found room
	time_t lowest = iter->data.time; //Lowest found time
	low = iter->data;

	while(iter != NULL)
	{
		if(lowest > iter->data.time)
		{
			prev = prev0;//Remember the previous node of the lowest found element to remove it.
			lowest = iter->data.time;
			low = iter->data;
			del = iter;//The node about to be returned and deleted
		}
	
		prev0 = iter;
		iter = iter->next;
				
	}
	
	prev->next = del->next;
	free( del );

		
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
	
	node* head = (node *) arg;
	while(1)
	{
		srand(time( NULL ));
		unsigned int timeout = random() % 5 + 1;

		sleep(timeout); //Sleep for n, where n is a random amount of time between 1 and 6 seconds

		int room = random() % 9999; //Random room Number, 0 < n < 10000
		struct node* head = (struct node*) arg;
		time_t alarm = time( NULL ) + random() % 60 + 10;

		//Create a room for the new time & room number
		struct room *place; 
		place = (struct room *)malloc(sizeof(struct room));
		
		//Assign the values.
		place->room = room;
		place->time = alarm;

		//If nothing waiting, it's the first alarm, therefore, the soonest.
		if( waiting == 0)
		{
			soonest.time = alarm;
			soonest.room = room;
			waiting++;
			printf("Registering:\t%4d %s\n", room, ctime(&alarm));
		}
		else
		{
			//Else if the Alarm is later than the soonest alarm, place in List.
			if(soonest.time < alarm)
			{
				add(head, *place);
				waiting++;
				printf("Registering:\t%4d %s\n", room, ctime(&alarm));
			}
			else
			{

				//Otherwise, it's the soonest, swap the current soonest into the list and place this new room into the soonest var.
				add(head, soonest);
				soonest = *place;
				waiting++;
				printf("Registering:\t%4d %s\n", room, ctime(&alarm));
				pthread_cond_signal(&cond); //Signal cond var that the soonest has been modified.
			}
		
		}

	}
}

static void* consumer(void *arg)
{
	//Initialisations
	int rc;
	struct timespec ts;

	//Repack the void* arg into the node* datatype.
	node* head = (node*) arg;
	while(1)
	{
		while(waiting != 0)
		{
			//Place the time_t into the timespec struct
			ts.tv_sec = soonest.time;
			ts.tv_nsec = 0;
			int room = soonest.room;
			
			//Wait for the soonest to time out or to be signalled with the &cond var.
			rc = pthread_cond_timedwait(&cond, &mutex, &ts);
			if(rc == ETIMEDOUT) //Timed out, the soonest is to be woken up now
			{
				printf("Wake Up:\t%4d %s\n", soonest.room, ctime(&ts.tv_sec));
				expired++;
				waiting--;
				printf("Expired Alarms:\t%d\nPending Alarms:\t%d\n\n", expired,waiting);
				rc = 2;
				soonest = search(head);// Get the earliest alarm available in the List and replace the soonest var.
				
				
			}
			else if(rc == 0)//New Soonest added, store soonest back in list
			{
				struct room *rm;
				rm = (struct room *)malloc(sizeof(struct room));
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

	
	struct node* nodes = NULL;
	nodes = (node *)malloc(sizeof(node));
	pthread_create(&alarmCreator, NULL, producer, (void *) nodes);
	pthread_create(&alarmPrinter, NULL, consumer, (void *) nodes);
	
	pthread_join(alarmCreator, NULL);

	return 0;
}
