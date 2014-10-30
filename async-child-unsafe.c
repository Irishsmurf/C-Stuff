#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void
handler(int signo)
{
  printf("This is the SIGUSR1 signal handler!\n");
}

int
main(void)
{
  sigset_t set;

  sigemptyset(&set);

  sigset(SIGUSR1, handler);

  sigprocmask(SIG_SETMASK, &set, NULL);

  while (1) {
    printf("This is main()!\n");
  }

  return (0);
}
