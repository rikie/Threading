#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

void *printEven();
void *printOdd();

int count = 0;
#define COUNT_DONE 200

int main() {
	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, &printOdd, NULL);
	pthread_create(&thread2, NULL, &printEven, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	exit(0);
}

void *printOdd() {
	for (;;) {
		pthread_mutex_lock(&count_mutex);
		if (count % 2 != 0) {
			pthread_cond_wait(&condition_var, &count_mutex);
		}
		count++;
		printf("%d\n",count);
		pthread_cond_signal(&condition_var);
		if(count >= COUNT_DONE) {
			pthread_mutex_unlock(&count_mutex);
			return NULL;
		}
		pthread_mutex_unlock(&count_mutex);
	}
}

void *printEven() {
	for(;;) {
		pthread_mutex_lock(&count_mutex);
		if(count % 2 == 0) {
			pthread_cond_wait(&condition_var, &count_mutex);
		}
		count++;
		printf("%d\n", count);
		pthread_cond_signal(&condition_var);
		if(count >= COUNT_DONE) {
			pthread_mutex_unlock(&count_mutex);
			return NULL;
		}
		pthread_mutex_unlock(&count_mutex);
	}
}
