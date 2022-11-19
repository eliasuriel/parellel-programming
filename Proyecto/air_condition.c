#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>

#define NUM_THREADS 5
#define N 6
#define F 7

unsigned int seed;
pthread_mutex_t lock;
int checa = 0;
//Mas facil la comparación con -1
int TempMatrix[N][F]={
    {-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1}
};

//Matriz hc
char MatrixHC[N][F]={
{'C','C','C','C','C','C','C'},
{'C','C','C','C','C','C','C'},
{'C','C','H','H','H','C','C'},
{'C','C','H','H','H','C','C'},
{'C','C','H','H','H','C','C'},
{'C','C','C','C','C','C','C'}
};


void  MatrizCompleta(){
  for(int i = 0; i < 6; i++){
    for(int  j= 0;  j< 7; j++){
      if (TempMatrix[i][j] == -1){
        return;
      }
    }
  }
  checa = 1;
}

void* LenaTempM(void* arg){
    
    while(true){
        int x = rand()%6;
        int y =rand()%7;
        if(arg == 0){
            MatrizCompleta();
        }
        if(checa == 1){
            pthread_exit(NULL);
        } 
    
        pthread_mutex_lock(&lock);
        if(MatrixHC[x][y] == 'C'){
            TempMatrix[x][y] = rand()%61;
        }
        else if(MatrixHC[x][y] == 'H') {
            TempMatrix[x][y] = 60+rand()%(101-60);
        }
        pthread_mutex_unlock(&lock);
    }
    
}




int main(int argc, char *argv[]) {
    int rc;
    //Imprimir la matriz de calor

    printf("Matriz HOT/COLD");
    printf("\n");
    for(int i = 0; i < N; i++){
        for(int j = 0; j < F; j++){
           printf("%c ",MatrixHC[i][j]);

        }
        printf("\n");
    }

    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\n mutex init failed\n");
        return 1;
    }
    //seed = time(NULL);
    int threadcnt = NUM_THREADS;
    if(argc > 1){
        threadcnt = atoi(argv[1]);
    }
    pthread_t threads[threadcnt];
    
    //omp_set_num_threads(threadcnt);
    // Checando que el omp esta definido
    //printf("OMP defined, threadcr = %d\n", threadcnt);

    srand(time(NULL));
    
    for(long c = 0;  c< NUM_THREADS; c++){
      printf("thread: %ld\n", c);
      rc = pthread_create(&threads[c], NULL, LenaTempM, (void*) c);
      if (rc){  
        printf("Error");
        break;
      }
    }
    printf("Matriz Temperatura");
    printf("\n");
    for(int l = 0; l < N; l++){
        for(int h = 0; h < F; h++){
           printf("%d ",TempMatrix[l][h]);
        }
        printf("\n");
    }
    //Ecuación de calor

    int max_iter_time = 80;
    int alpha = 2;
    int delta_x = 1;
    
    float delta_t = (pow(delta_x, 2))/(4 * alpha);
    float gamma = (alpha * delta_t) / (delta_x*delta_x);
    int u[max_iter_time][N][F];

    #pragma omp parallel for private(k, i, j) shared (u, TempMatrix)
    for(int k = 0; k < max_iter_time; k++){
        for(int i = 0; i < N; i++){
            for(int j = 0; j < F; j++){
                u[k][i][j] = TempMatrix[i][j];
            }
        }
    }
    #pragma omp parallel for private(k, i, j) shared (u)
    for(int k = 0; k < max_iter_time - 1; k++){
        for(int i = 1; i < N - 1; i+= delta_x){
            for(int j = 1; j < F - 1; j+= delta_x){
                u[k + 1][i][j] = gamma * (u[k][i+1][j] + u[k][i-1][j] + u[k][i][j+1] + u[k][i][j-1] - 4*u[k][i][j]) + u[k][i][j];
            }
        }
    }
    printf("\nEsta es la ecuación de la fabrica: ");
    printf("\n y = %d * (%f / (%d)^2)\n",alpha,delta_t,delta_x);
    //esto va al final;
    
    printf("Termino todo");
    pthread_mutex_destroy(&lock);
    return 0;
}
