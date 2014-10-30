#include <stdio.h>
#include <stdlib.h>

int main()
{
	time_t before = time( NULL );
	sleep(1);
	time_t after = time( NULL);

	if(before < after)
	{
		printf("Before did come before after");
	}
	else
		printf("What the fuck");

	if(after > before)
	{
		printf("Yarp Yarp\n");
	}
}
