/*

Elías Uriel Velázquez Rojas_A01639716

Con apoyo de los siguientes repositorios:
https://github.com/VictorRodriguez/operating-systems-lecture/blob/master/labs/04/pi.c
https://github.com/VictorRodriguez/parallel-programming-lecture/blob/main/labs/04/practice1.c
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_THREADS 75

int puntos = 100000;
int cont_circle = 0;
double radio=1;

pthread_mutex_t lock;
int pointsThread;

unsigned int seed;

double Numrandom(){
    return (double)rand_r(&seed) / (double)((unsigned)RAND_MAX + 1);
}

int en_circulo(double x, double y){
    double d = pow(radio,2) - (pow(x,2) + pow(y,2));
    if(d>=0){
        return 1;
    }else{
        return 0;
    }
}

void * Count_montecarlo(void * a){
    int contador = 0;
    pthread_mutex_lock(&lock);
    int j;
    for( j=0; j<pointsThread; j++){
        double x = Numrandom();
        double y = Numrandom();
        int circulo = en_circulo(x,y);
        if(circulo ==1){
            cont_circle= cont_circle + 1;
        }else{
          contador++;
        }
    }
     pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char *argv[]){
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\n mutex init failed\n");
            return 1;
    }
    //do j = 1,npoints
    seed = time(NULL);

    pthread_t threads[NUM_THREADS];
    pointsThread= puntos/NUM_THREADS;

    int i;
    for( i=0; i<NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, Count_montecarlo, NULL);
    }
    int a;
    for(a=0; a<NUM_THREADS; a++){
        pthread_join(threads[a], NULL);
    }

    double PI = 4.0*(double)cont_circle/(double)puntos;
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("PI = %f\nTime used=%f\n",PI, cpu_time_used);
    pthread_mutex_destroy(&lock);
}
