#include <pthread.h>
#include <signal.h>
#include <stdio.h>


void *one(void *arg)
{
	int catch;
	int i = 0;
	int target = *((int *)arg);
	sigset_t signal_catch;
	sigemptyset(&signal_catch);
	sigaddset(&signal_catch, SIGUSR1);
	while(i <= target)
	{
			printf("Evens Waiting...\n");
			sigwait(&signal_catch, &catch);
			printf("Evens!: %d\n", i);
			i = i + 2;
			raise(SIGUSR2);	
	}
}

void *two(void *arg)
{
	int catch;
	int target = *((int *)arg);
	int i = 1;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,  SIGUSR1);
	while(i < target)
	{
		printf("Odds Waiting..\n");
		sigwait(&set, &catch);
		printf("Odd!: %d\n", i);
		i = i + 2;
		raise(SIGUSR2);
	}
}
int main (int argi, char * argv[])
{
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	//sigaddset(&set, SIGUSR2);	
	pthread_sigmask(SIG_BLOCK, &set, NULL);
	sigaddset(&set, SIGUSR2);
//	sigaddset(&set, SIGUSR2);
	int i;
	int catch;
	pthread_t ones;
	pthread_t twos;
	
	int param;

	if(argi > 1)
		param = atoi(argv[1]);
	else
	{
		printf("Requires a integer parameter.\n");
		return 0;
	}
	

	pthread_create(&ones, NULL, one, (void *)&param);
	pthread_create(&twos, NULL, two, (void *)&param);

	for(i = 0; i <= param; i++)
	{
		if(i % 2 == 0)
		{
			printf("Even Signal...\n");
			pthread_kill(ones, SIGUSR1);
		}
		else
		{
			printf("Odd signal...\n");
			pthread_kill(twos, SIGUSR1);
		}	
		

		sigwait(&set, &catch);
	}

	//pthread_kill(ones, SIGTERM);
	//pthread_kill(twos, SIGTERM);

	pthread_join(ones, NULL);
	pthread_join(twos, NULL);
	return 0;
}
