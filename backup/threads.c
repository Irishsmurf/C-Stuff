#include <pthread.h>
#include <signal.h>
#include <stdio.h>


void *even(void *arg)
{
	int catch;
	int i = 0;
	pthread_t target = *((pthread_t *)arg);
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	while(1)
	{
	//		printf("Evens Waiting...\n");
			sigwait(&set, &catch);
			printf("Evens!: %d\n", i);
			i = i + 2;
			pthread_kill(target, SIGUSR2);
			//raise(SIGUSR2);	
	}
}

void *odd(void *arg)
{
	int catch;
	pthread_t target = *((pthread_t *)arg);
	int i = 1;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,  SIGUSR1);
	while(1)
	{
		//printf("Odds Waiting..\n");
		sigwait(&set, &catch);
		printf("Odd!: %d\n", i);
		i = i + 2;
		pthread_kill(target, SIGUSR2);
		//raise(SIGUSR2);	
	
	}
}

void *watch(void *arg)
{
	int catch;
	int i;
	int target = *((int *) arg);
	sigset_t set;
	sigemptyset;
	sigaddset(&set, SIGUSR2);
	
	pthread_t evens;
	pthread_t odds;
	pthread_t self = pthread_self();
	pthread_create(&odds, NULL, odd, (void *)&self);
	pthread_create(&evens, NULL, even, (void *)&self);

	
	
	for(i = 0; i <= target; i++)
	{
		if(i % 2 == 0)
		{
//			printf("%d\n", i);
			pthread_kill(evens, SIGUSR1);
		}
		else
			pthread_kill(odds, SIGUSR1);
		
	//	printf("Waiting for SIGUSR2\n");
		sigwait(&set, &catch);
	}
	
}
int main (int argi, char * argv[])
{
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);	
	pthread_sigmask(SIG_BLOCK, &set, NULL);
	
	sigaddset(&set, SIGUSR2);
	int i;
	int catch;
	pthread_t watcher;
	
	int param;

	if(argi > 1)
		param = atoi(argv[1]);
	else
	{
		printf("Requires a integer parameter.\n");
		return 0;
	}

	pthread_create(&watcher, NULL, watch, (void *)&param);
	pthread_join(watcher, NULL);
	printf("laters o/");
	return 0;
}
