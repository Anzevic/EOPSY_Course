#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N		5
#define LEFT	( i + N - 1 ) % N
#define RIGHT	( i + 1 ) % N
#define THINKING	    1
#define HUNGRY		    2
#define EATING		    3
#define EATING_TIME	    1
#define THINKING_TIME	2
#define FOOD_LIMIT	    4

#define	up		pthread_mutex_unlock
#define	down		pthread_mutex_lock

void grab_forks(int i);
void put_away_forks(int i);
void test(int i);
void* philos_lifecycle(void* num);

pthread_mutex_t m;
pthread_mutex_t s[N];
int state[N];
pthread_t philos[N];

int main(){
    int i;
	pthread_mutex_init(&m, NULL);
	for(i = 0; i < N; i++){
		state[i]=THINKING;
		pthread_mutex_init(&s[i], NULL);
		down(&s[i]);

		if(pthread_create(&philos[i], NULL, philos_lifecycle, (void*)(+i))){
            printf("failed to create thread");
            return 1;
        }
	}
	for(i = 0; i < N; i++){
		pthread_join(philos[i],NULL);
	}
	pthread_mutex_destroy(&m);
	for(i = 0; i < N; i++){
		pthread_mutex_destroy(&s[i]);
	}
	pthread_exit(NULL);
	return 0;
}


void grab_forks( int i ){
	down(&m);
		state[i]=HUNGRY;
		test(i);
	up(&m);
	down(&s[i]);
}

void put_away_forks( int i ){
	down(&m);
		state[i]=THINKING;
		test(LEFT);
		test(RIGHT);
	up(&m);
}

void test( int i ){
	if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
		state[i] = EATING;
		up(&s[i]);
	}
}

void* philos_lifecycle(void* num){
    int* i = (int*)num;
    printf("Philosopher %d came to the table.\n", *i);
    while(1){
	printf("Philosopher %d is thinking.\n", *i);
        sleep(THINKING_TIME);
        grab_forks(*i);
        printf("Philosopher %d has started eating.\n", *i);
        sleep(EATING_TIME);
        put_away_forks(*i);
    }
    return NULL;
}
