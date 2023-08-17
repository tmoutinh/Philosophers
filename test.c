#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thread_function() {
    printf("Thread is running...\n");
    sleep(2);
    printf("Thread is done.\n");
    return NULL;
}

int main() {
    pthread_t thread;

    // Create the thread
    pthread_create(&thread, NULL, thread_function, NULL);


    // Detach the thread
    pthread_detach(thread);


    printf("Main thread is continuing...\n");

    // Sleep for a while to let the detached thread run
    sleep(3);

    printf("Main thread is done.\n");

    return 0;
}