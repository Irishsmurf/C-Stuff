#include <pthread.h>
#include <signal.h>
#include <stdio.h>

void even_handler(void *arg)
{
	printf("Goodbye from Evens!\n");
}

void odd_handler(void *arg)
{
	printf("Goodbye from Odd!\n");
}
//Even Thread
void *even(void *arg)
{
	//link cancel handle
	pthread_cleanup_push(even_handler, NULL);
	int catch;
	int i = 0;
	pthread_t target = *((pthread_t *)arg);
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	while(1)
	{
			sigwait(&set, &catch);//Wait for signal
			printf("Even!: %d\n", i);
			i = i + 2;
			pthread_kill(target, SIGUSR2);//Signal Watch thread.
	}
	pthread_cleanup_pop(1);
	return NULL;
}
//Thread responsible for Odd Numbers
void *odd(void *arg)
{
	pthread_cleanup_push(odd_handler, NULL);
	int catch;
	pthread_t target = *((pthread_t *)arg);//Cast the Argument as a pthread_t
	int i = 1;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,  SIGUSR1);
	while(1)
	{
		sigwait(&set, &catch);//wait for signal
		printf("Odd!:  %d\n", i);
		i = i + 2;//Increment to next odd number
		pthread_kill(target, SIGUSR2);//Send reponse signal. Everything is done here.
	}
	pthread_cleanup_pop(1);
	return NULL;
}

void *watch(void *arg)
{
	int catch, i;

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
		if(i % 2 == 0)//if Even
		{
			pthread_kill(evens, SIGUSR1);//Signal Even to print
		}
		else
			pthread_kill(odds, SIGUSR1); //Else signal odd to print
		
		sigwait(&set, &catch); // Wait for reponse
	}
//Cancel both threads
	pthread_cancel(odds);
	pthread_cancel(evens);
	pthread_join(odds, NULL);
	pthread_join(evens, NULL);

}
int main (int argi, char * argv[])
{
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);	
	pthread_sigmask(SIG_BLOCK, &set, NULL);
	int i, catch, param;
	pthread_t watcher;
	
	if(argi > 1)
		param = atoi(argv[1]);
	else
	{
		printf("Requires a integer parameter.\n");
		return 0;
	}

	pthread_create(&watcher, NULL, watch, (void *)&param);
	pthread_join(watcher, NULL);
	
	return 0;
}
