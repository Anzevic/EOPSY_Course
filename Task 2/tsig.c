#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_CHILD 7 

void signalHandler(int signalNumber){
	printf("Signal %d was captured: invokation started\n", signalNumber);
	exit(1);
}

int main( int argc, char *argv[] ){

	pid_t pid[NUM_CHILD];

	printf("parent_pid[%d] \n", getpid());

	int i=0;
	int suspended = 0;
	
	for (; i<NUM_CHILD; i++){
		pid[i] = fork();
		
		if (pid[i] == 0){
			printf("child_pid[%d] - parent's id: %d.\n", getpid(), getppid());
			sleep(20);
			exit(0);
		}
		else if (pid[i] == -1){
			fprintf(stderr, "error: %d \n", errno);
			
			signal(SIGTERM, signalHandler);
		}
		
		sleep(5); 
	}

	printf("parent_pid[%d] \n", getpid());

	
	
	for (i = 0; i<NUM_CHILD; i++) {
		
		int status;
		if ( waitpid(pid[i], &status, 0) == -1 ) {
			perror("waitpid failed");
			
			return EXIT_FAILURE;
		}

		if ( WIFEXITED(status) ) {
			const int es = WEXITSTATUS(status);
					suspended++;
			printf("parent_pid[%d]: child_exit_status[%d]  %d\n",getpid(), pid[i], es);
		}
	}
	printf("Number of suspended processes: %d \n",suspended);

	printf("Successful execution\n");

	return 0;
}