#include <stdio.h>

#define MAX_PROCESOS 5
#define MAX_RECURSOS 3

int recursos_disponibles[MAX_RECURSOS];
int matriz_asignacion[MAX_PROCESOS][MAX_RECURSOS];
int matriz_maxima[MAX_PROCESOS][MAX_RECURSOS] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3}
};
int matriz_necesidad[MAX_PROCESOS][MAX_RECURSOS];
int procesos_finalizados[MAX_PROCESOS];

void inicializar_recursos_disponibles(int recursos_disponibles[], int max_recursos[]) {
    for (int i = 0; i < MAX_RECURSOS; ++i) {
        recursos_disponibles[i] = max_recursos[i];
    }
}

void inicializar_matriz_asignacion(int matriz[MAX_PROCESOS][MAX_RECURSOS]) {
    for (int i = 0; i < MAX_PROCESOS; ++i) {
        for (int j = 0; j < MAX_RECURSOS; ++j) {
            matriz[i][j] = 0;
        }
    }
}

void inicializar_matriz_necesidad() {
    for (int i = 0; i < MAX_PROCESOS; ++i) {
        for (int j = 0; j < MAX_RECURSOS; ++j) {
            matriz_necesidad[i][j] = matriz_maxima[i][j] - matriz_asignacion[i][j];
        }
    }
}

void solicitar_recursos(int proceso, int solicitud[MAX_RECURSOS]) {
    for (int i = 0; i < MAX_RECURSOS; ++i) {
        if (solicitud[i] > matriz_necesidad[proceso][i] || solicitud[i] > recursos_disponibles[i]) {
            printf("Solicitud inválida para el proceso %d\n", proceso);
            return;
        }
    }

    for (int i = 0; i < MAX_RECURSOS; ++i) {
        recursos_disponibles[i] -= solicitud[i];
        matriz_asignacion[proceso][i] += solicitud[i];
        matriz_necesidad[proceso][i] -= solicitud[i];
    }

    if (verificar_asignacion_segura()) {
        printf("Solicitud aceptada para el proceso %d\n", proceso);
    } else {
        for (int i = 0; i < MAX_RECURSOS; ++i) {
            recursos_disponibles[i] += solicitud[i];
            matriz_asignacion[proceso][i] -= solicitud[i];
            matriz_necesidad[proceso][i] += solicitud[i];
        }
        printf("Solicitud rechazada para el proceso %d (podría causar un deadlock)\n", proceso);
    }
}

int verificar_asignacion_segura() {
    int trabajo[MAX_RECURSOS];
    int procesos_terminados = 0;

    for (int i = 0; i < MAX_RECURSOS; ++i) {
        trabajo[i] = recursos_disponibles[i];
    }

    for (int i = 0; i < MAX_PROCESOS; ++i) {
        procesos_finalizados[i] = 0;
    }

    while (procesos_terminados < MAX_PROCESOS) {
        int proceso_encontrado = 0;

        for (int i = 0; i < MAX_PROCESOS; ++i) {
            if (!procesos_finalizados[i]) {
                int recursos_suficientes = 1;

                for (int j = 0; j < MAX_RECURSOS; ++j) {
                    if (matriz_necesidad[i][j] > trabajo[j]) {
                        recursos_suficientes = 0;
                        break;
                    }
                }

                if (recursos_suficientes) {
                    proceso_encontrado = 1;
                    procesos_finalizados[i] = 1;

                    for (int j = 0; j < MAX_RECURSOS; ++j) {
                        trabajo[j] += matriz_asignacion[i][j];
                    }

                    break;
                }
            }
        }

        if (!proceso_encontrado) {
            return 0;
        }

        procesos_terminados++;
    }

    return 1;
}

int main() {
    int max_recursos[MAX_RECURSOS] = {10, 5, 7};
    inicializar_recursos_disponibles(recursos_disponibles, max_recursos);

    inicializar_matriz_asignacion(matriz_asignacion);
    inicializar_matriz_necesidad();

    solicitar_recursos(0, (int[]){3, 2, 2});
    solicitar_recursos(1, (int[]){1, 0, 2});
    solicitar_recursos(2, (int[]){1, 3, 1});
    solicitar_recursos(3, (int[]){0, 0, 2});

    return 0;
}

