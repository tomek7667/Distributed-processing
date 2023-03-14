#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

int global_counter = 0;
sem_t sem;

#define WORKCOUNT 40000000

void* inc(void* vp) {
    int i = WORKCOUNT;
    while (i--) {
        // Lookup: critical section (semaphores, mutexes)

        sem_wait(&sem);
        global_counter++;
        sem_post(&sem);
    }
}

int main()
{
    pthread_t t1, t2, t3, t4;
    printf("%d\n", global_counter);
    // inc(NULL);
    pthread_create(&t1, NULL, inc, NULL);
    pthread_create(&t2, NULL, inc, NULL);
    pthread_create(&t4, NULL, inc, NULL);
    pthread_create(&t2, NULL, inc, NULL);
    // Joining the threads
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    printf("%d\n", global_counter);
    return 0;
}
