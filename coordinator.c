#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#define ONE_SECOND 1000000


static void* count_evens(void *arg)
{

	int i;
	int target = *((int *)arg);
	for(i = 0; i <= target; i++) 
		if(i % 2 == 0)
		{
			(void)printf("Evens!: %d\n", i);
			sigwait();
		}
	return NULL;
}


static void* count_odds(void *arg)
{
	int i;
	int target = *((int *)arg);
	for(i = 0; i <= target; i++)
		if(i % 2 == 1)
		{
			(void)printf("Odd!:   %d\n", i);
			sleep(1);
		}
	return NULL;
}
				 

static void wake_print(pthread_t thread)
{
	sigset_t set;
	sigemptyset(&set);
	
}

int main(int argi, char * argv[])
{
	int isEven = 1;
	int arg = 10;
	
	sigset_t set;
	sigemptyset(&set);
	sigset(SIGUSR1, count_evens);
	sigset(SIGUSR2, count_odds);

	pthread_t evens;
	pthread_t odds;
	
	if(argi > 1)
		arg = atoi(argv[1]);
	printf("%d\n", arg);
	
	

	pthread_create(&evens, NULL, count_evens, (void*) &arg);
	pthread_create(&odds, NULL, count_odds, (void*)&arg);

	pthread_join(evens, NULL);
	return 0;

}
