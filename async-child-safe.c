#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

static void *
handlerthread(void *in)
{
  sigset_t set;
  int sig;

  sigemptyset(&set);

  sigaddset(&set, SIGUSR1);

  while (1) {
      sigwait(&set, &sig);
      printf("We received a SIGUSR1!\n");
  }

  return ((void *)NULL);
}

int
main(void)
{
  pthread_t t;

  pthread_create(&t, NULL, handlerthread, (void *)NULL);

  while (1) {
   printf("This is main()!\n");
   //pthread_kill(t, SIGUSR1);
  }

  return (0);
}
