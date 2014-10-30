#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


typedef room
{
	int num;
	time_t time;
} room_t;

typedef struct 
{
	int* buffer;
	int size;
	int occupied;
	int nextin;
	int nextout;
	int ins;
	int outs;
	pthread_mutex_t mutex;
	pthread_cond_t more;
	pthread_cond_t less;
} bb_t;

static int init(bb_t* bb, int size)
{
	//Set ever interal variable to 0;
	bb->occupied = bb->nextin = bb->nextout = bb->ins = bb->outs = 0;
	bb->size = size; //Set the size;

	bb->buffer = (int* )malloc(bb->size* sizeof(*bb->buffer));
	
	if(bb->buffer = NULL)
	{
		perror("malloc()");
		exit(EXIT_FAILURE);
	}

	pthread_mutex_init(&bb->mutex, NULL);
	pthread_cond_init(&bb->more, NULL);
	pthread_cond_init(&bb->less, NULL);

	srand((unsigned int) getpid());

	return (0);
}

void alarm(void *arg)
{
	room_t room = *((room_t*) arg);
}

void alarmCreator(void *arg)
{
	bb_t buf = *((bb_t*) arg);

	srand( time(NULL) );
	int room = random() % 6000;                 //Random number, from 0 - 6000

	time_t timer = time(NULL) + random() % 60; //One Day
	printf("Registering::\t%d %s\n", room, ctime(&timer));
	
	pthread_mutex_lock(&buf->mutex);
	while(&buf->occupied ==&buf->size)
	{
		pthread_cond_wait(&buf->less, &buf->mutex);
	}

	//buf->buffer[buf->nextin] = 
	
	
}

int main()
{
	bb_t buffer;
	init(&buffer, 200);
	alarmCreator((void *) &buffer);
}


