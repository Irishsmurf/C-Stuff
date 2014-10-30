#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main()
{
  int i, j;
  pid_t pid;

  for (i = 1; i <= 10; i++) {

    pid = fork();

    if (pid == 0) {
      return (i);
    } else {
      printf("Created child with ID: %d\n", pid);
    }
  }

  for (i = 0; i < 10; i++) {
    pid = wait(&j);
    printf("%d returned %d\n", pid, WEXITSTATUS(j));
  }

  sleep(99999);

  return (0);
}
