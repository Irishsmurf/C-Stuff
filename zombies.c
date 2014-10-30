#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main()
{
  int i;
  pid_t pid;

  for (i = 1; i <= 10; i++) {

    pid = fork();

    if (pid == 0) {
      return (i);
    } else {
      printf("Created child with ID: %d\n", pid);
    }
  }

  sleep(99999);

  return (0);
}
