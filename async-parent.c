#include <unistd.h>
#include <signal.h>

int
main(int argc, char *argv[])
{
  pid_t pid;
  sigset_t set;

  sigemptyset(&set);

  sigaddset(&set, SIGUSR1);

  sigprocmask(SIG_BLOCK, &set, NULL);

  pid = fork();

  if (pid == 0) {
    execlp(argv[1], NULL);
  }

  while (1) {
    kill(pid, SIGUSR1);
  }

  return (0);
}
