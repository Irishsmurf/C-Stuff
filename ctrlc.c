#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void
handler(int signo)
{
  printf("You can't kill me!\n");
}

int
main(void)
{
  sigset_t set;

  sigemptyset(&set);

  sigset(SIGINT, handler);

  while (1) {

    sigsuspend(&set);
  
  }

  return (0);
}
