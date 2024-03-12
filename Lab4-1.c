#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t recurso1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t recurso2 = PTHREAD_MUTEX_INITIALIZER;

void *proceso1(void *arg) {
    while (1) {
        pthread_mutex_lock(&recurso1);
        printf("Proceso 1 adquirió recurso 1\n");
        sleep(1);

        printf("Proceso 1 intentando adquirir recurso 2...\n");
        pthread_mutex_lock(&recurso2);
        printf("Proceso 1 adquirió recurso 2\n");
        sleep(1);

        pthread_mutex_unlock(&recurso1);
        pthread_mutex_unlock(&recurso2);
    }

    return NULL;
}

void *proceso2(void *arg) {
    while (1) {
        pthread_mutex_lock(&recurso2);
        printf("Proceso 2 adquirió recurso 2\n");
        sleep(1);

        printf("Proceso 2 intentando adquirir recurso 1...\n");
        pthread_mutex_lock(&recurso1);
        printf("Proceso 2 adquirió recurso 1\n");
        sleep(1);

        pthread_mutex_unlock(&recurso2);
        pthread_mutex_unlock(&recurso1);
    }

    return NULL;
}

int main() {
    pthread_t hilo_proceso1, hilo_proceso2;

    pthread_create(&hilo_proceso1, NULL, proceso1, NULL);
    pthread_create(&hilo_proceso2, NULL, proceso2, NULL);

    pthread_join(hilo_proceso1, NULL);
    pthread_join(hilo_proceso2, NULL);

    return 0;
}

