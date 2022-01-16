
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
volatile atomic_long cnt = 0;
typedef struct mycnt_t {
    long cntA;
    long cntB;
}mycnt_t;

typedef _Atomic mycnt_t atomic_mycnt_t;


volatile  atomic_mycnt_t mycnt;
//volatile long cnt = 0;
void *thread(void *vargp) {
    long niters = *((long *)vargp);
    for (long i = 0; i < niters; i++) {
        cnt++;
        mycnt.cntA++;
    }
    return NULL;
}

int main(int argc, char **argv) {
    long niters;
    pthread_t tid1, tid2;
    if(argc > 1)
        niters = atoi(argv[1]);
    else
        niters = 10000;
    pthread_create(&tid1, NULL, thread, &niters);
    pthread_create(&tid2, NULL, thread, &niters);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    if (cnt != (2 * niters) || mycnt.cntA != (2 * niters))
        printf("Wrong! cnt=%ld, mycnt.cnt=%ld\n", cnt, mycnt.cntA);
    else
        printf("Correct! cnt=%ld, mycnt.cnt=%ld\n", cnt, mycnt.cntA);
    return 0;
}