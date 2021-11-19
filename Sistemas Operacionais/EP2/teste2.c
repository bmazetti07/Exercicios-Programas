#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

void * fun (void * arguments) {
    pthread_cancel (pthread_self ());
}

int main () {
    pthread_t thread;

    pthread_create (&thread, NULL, fun, NULL);
    pthread_cancel (thread);

    return 0;
}