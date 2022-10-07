
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_THREADS 1

int npoints = 100000;
int cont_circle = 0;
double radio=1;
pthread_mutex_t lock;
int pointsThread;

unsigned int seed;

double random(){
    return (double)rand_r(&seed) / (double)((unsigned)RAND_MAX + 1);
}

int find_circle(double x, double y){
    double d = pow(radio,2) - (pow(x,2) + pow(y,2));
    if(d>=0){
        return 1;
    }else{
        return 0;
    }
}

void * Count_montecarlo(void *t_id){
    double x, y;
    int contador = 0;
    pthread_mutex_lock(&lock);
    for(int i=0; i<pointsThread; i++){
        x = random();
        y = random();
        int circulo = find_circle(x,y);
        if( circulo ==1){
            cont_circle= cont_circle + 1;
        }else{
          contador = contador + 1;
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
    pointsThread= npoints/NUM_THREADS;


    for(int i=0; i<NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, Count, NULL);
    }

    for(int i=0; i<NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    double PI = 4.0*(double)circle_count/(double)npoints;
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("PI = %f\nTime used=%f",PI, cpu_time_used);
    pthread_mutex_destroy(&lock);
}
