#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    int fd;

    fd = open(argv[1], O_RDWR);

    printf("fd: %d\n", fd);

    if (fd < 0) {
	perror("open()");
    }

    close(fd);

    return (0);
}
