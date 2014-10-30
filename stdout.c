#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
  char string[] = "Hello everybody!\n";

  write(1, string, sizeof (string));

  return (0);
}
