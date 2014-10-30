#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/* Our SIGALRM handler is invoked on receipt of SIGALRM */
static void
handler(int signo)
{
  /* Write out a message */
  printf("Alarm received\n");

  /* Schedule another alarm for 5 seconds */
  alarm(5);
}

/* Install signal handler and sleep until signal arrives */
int
main(void)
{
  sigset_t	set;

  /* Zero the bitmask */
  sigemptyset(&set);

  /* Associate the handler with SIGALRM */
  sigset(SIGALRM, handler);

  /* Schedule first alarm (handler schedules further ones) */
  alarm(5);

  while (1) {

    /* Set signal mask to set and wait for an unblocked signal */
    sigsuspend(&set);
  }

  return (0);
}
