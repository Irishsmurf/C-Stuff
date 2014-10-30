#include <signal.h>
#include <unistd.h>

static void handler(int signo)
{
	printf("Wake-up!\n");

	raise(SIGUSR1);
}

int main()
{
	sigset_t set;
	sigemptyset(&set);
	sigset(SIGUSR1, handler);
	sleep(1);
	raise(SIGUSR1);


	while(1)
	{
		sigsuspend(&set);
		printf("wat");
	}

	return 0;
}
