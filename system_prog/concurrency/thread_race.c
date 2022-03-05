#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


void *thread(void *vargp) {
    int myid = *((int *)vargp);
    free(vargp);
    printf("Thread ID %d\n", myid);
    return NULL;
}

#define N 10
int main(int argc, char const *argv[])
{
    pthread_t tid[N];
    for(int i = 0; i < N; i++) {
        // Use local storage to eliminate share state;
        int *local = malloc(sizeof(int));
        *local = i;
        //pthread_create(&tid[i], NULL, thread, &i);
        pthread_create(&tid[i], NULL, thread, local);

    }
    for(int i = 0; i < N; i++) {
        pthread_join(tid[i], NULL);
    }
    return 0;
}
