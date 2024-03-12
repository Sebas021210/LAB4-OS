#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5
sem_t tenedores[NUM_FILOSOFOS];

void pensar(int id) {
    printf("Filósofo %d está pensando...\n", id);
    sleep(rand() % 3 + 1);
}

void comer(int id) {
    printf("Filósofo %d está comiendo.\n", id);
    sleep(rand() % 3 + 1);
}

void tomarTenedores(int id) {
    sem_wait(&tenedores[id]);
    sem_wait(&tenedores[(id + 1) % NUM_FILOSOFOS]);
}

void dejarTenedores(int id) {
    sem_post(&tenedores[id]);
    sem_post(&tenedores[(id + 1) % NUM_FILOSOFOS]);
}

void *filosofo(void *arg) {
    int id = *((int *)arg);

    while (1) {
        pensar(id);
        tomarTenedores(id);
        comer(id);
        dejarTenedores(id);
    }

    return NULL;
}

int main() {
    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        sem_init(&tenedores[i], 0, 1);
    }

    pthread_t filosofos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];

    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        ids[i] = i;
        pthread_create(&filosofos[i], NULL, filosofo, &ids[i]);
    }

    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        pthread_join(filosofos[i], NULL);
    }

    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        sem_destroy(&tenedores[i]);
    }

    return 0;
}

