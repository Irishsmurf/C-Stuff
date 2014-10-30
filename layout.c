#include <stdio.h>
#include <stdlib.h>

int x = 33;

int
main()
{
    int i;
    char *j;
    int *p = (int *)0xDDDDDDDD;

    printf("Address of i: %p\n", (void *)&i);

    j = malloc(100);

    printf("Address of j: %p\n", (void *)j);

    printf("Address of x: %p\n", (void *)&x);

    printf("Address of main: %p\n", (void *)main);

    *p = 5;

    return (0);
}
