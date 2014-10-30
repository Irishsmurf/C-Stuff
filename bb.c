/* Solving the usual bounded buffer problem */
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "bb.h"

#define ONE_SECOND 1000000
#define RANGE 10
#define PERIOD 2

/* Our producer threads will each execute this function */
static void *
producer(void *bb_in)
{
  bb_t *bb;
  unsigned int seed;

  /* Convert what was passed in to a pointer to a bounded buffer */
  bb = (bb_t *)bb_in;

  /* Loop */
  while (1) {

    /* Sleep for up to 1s */
    usleep(rand_r(&seed) % ONE_SECOND);

    /* Acquire the lock */
    pthread_mutex_lock(&bb->mutex);

    /* While full wait until there is room available */
    while (bb->occupied == bb->size) {
      pthread_cond_wait(&bb->less, &bb->mutex);
    }

    /* Insert an item */
    bb->buffer[bb->nextin] = rand_r(&seed) % RANGE;

    /* Increment counters */
    bb->occupied++;
    bb->nextin++;
    bb->nextin %= bb->size;
    bb->ins++;

    /* Someone may be waiting on data to become available */
    pthread_cond_signal(&bb->more);

    /* Release the lock */
    pthread_mutex_unlock(&bb->mutex);
  }

  return ((void *)NULL);
}

/* Our consumer threads will each execute this function */
static void *
consumer(void *bb_in)
{
  bb_t *bb;
  unsigned int seed;

  /* Convert what was passed in to a pointer to a bounded buffer */
  bb = (bb_t *)bb_in;

  while (1) {

    /* Sleep for up to 1s */
    usleep(rand_r(&seed) % ONE_SECOND);

    /* Acquire the lock */
    pthread_mutex_lock(&bb->mutex);

    /* While empty wait until there is data available */
    while (bb->occupied == 0) {
      pthread_cond_wait(&bb->more, &bb->mutex);
    }

    /* Increment counters */
    bb->occupied--;
    bb->nextout++;
    bb->nextout %= bb->size;
    bb->outs++;

    /* Someone may be waiting on room to become available */
    pthread_cond_signal(&bb->less);

    /* Release the lock */
    pthread_mutex_unlock(&bb->mutex);
  }

  return ((void *)NULL);
}

/* Our monitor thread will each execute this function */
static void *
monitor(void *bb_in)
{
  bb_t	*bb;

  /* Convert what was passed in to a pointer to a bounded buffer */
  bb = (bb_t *)bb_in;

  while (1) {

    /* Pause */
    sleep(PERIOD);

    /* Acquire the lock */
    pthread_mutex_lock(&bb->mutex);

    printf("Delta: %d\n", bb->ins - bb->outs - bb->occupied);

    /* Release the lock */
    pthread_mutex_unlock(&bb->mutex);
  }

  return ((void *)NULL);
}

/* Initialisation */
static int
init(bb_t *bb, int size)
{
  /* Set up the bounded buffer internals */
  bb->occupied = bb->nextin = bb->nextout = bb->ins = bb->outs = 0;
  bb->size = size;

  /* Initialise our data structure */
  bb->buffer = (int *)malloc(bb->size * sizeof (*bb->buffer));

  /* Check malloc succeeded */
  if (bb->buffer == NULL) {
    perror("malloc()");
    exit(EXIT_FAILURE);
  }

  /* Initialise lock and condition variables */
  pthread_mutex_init(&bb->mutex, NULL);
  pthread_cond_init(&bb->more, NULL);
  pthread_cond_init(&bb->less, NULL);  

  /* Seed random number generator */
  srand((unsigned int)getpid());

  return (0);
}

int
main(int argc, char *argv[])
{
  pthread_t p, c, m;
  bb_t bb;

  /* Check usage */
  if (argc != 2) {
    printf("Usage: %s buffer_size\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* Initialise */
  init(&bb, atoi(argv[1]));

  /* Create our threads */
  pthread_create(&p, NULL, producer, (void *)&bb);
  pthread_create(&c, NULL, consumer, (void *)&bb);
  pthread_create(&m, NULL, monitor, (void *)&bb);

  /* Wait for our threads */
  pthread_join(p, NULL);
  pthread_join(c, NULL);
  pthread_join(m, NULL);

  return (0);
}
