#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include "mpi.h"

#define MIN 1
#define MAX 50

void llenar_Matriz(int *dim, int **matriz);
void mostrar_matriz(int *dim, int **matriz);
int buscarVecesNumeros(int *dim, int **matriz, int *consulta);
void entradas(int proceso, int nro_procesos, int  *dim, int *consulta);

int main(){
 srand(time(NULL));
 // variables
 int dim , consulta;
 int mas = 0;
 int proceso, nros_procesos;
 int aux;
// crear matriz de forma dinamica
// mpi 
MPI_Init(NULL, NULL);
MPI_Comm_rank(MPI_COMM_WORLD, &proceso);
MPI_Comm_size(MPI_COMM_WORLD, &nros_procesos);
entradas(proceso, nros_procesos, &dim, &consulta);

 int **matriz1 = (int **)malloc(dim * sizeof(int *));
 for (int i = 0; i < dim; i++) {
    matriz1[i] = (int *)malloc(dim * sizeof(int));
  }
 
if (proceso == 0){    
    for (int  i = 1; i < nros_procesos; i++){
        MPI_Recv( 
        &mas , 
        1, 
        MPI_INT , 
        i, 
        0, 
        MPI_COMM_WORLD , 
        MPI_STATUS_IGNORE);
        aux += mas;
    }
    printf("El número %d se repite %d veces en la matriz \n", consulta, aux);
}

if (proceso == 1  ){
    llenar_Matriz(&dim,matriz1);// esta parte llamo a la funcion
    printf("\nLa matriz generada es la siguiente \n");
    mostrar_matriz(&dim, matriz1);// esta parte llamo a la funcion para que me muestre la matriz
    printf("**************************************\n");  
    printf("\n");
}

if (proceso !=0 || proceso !=1){
    mas = buscarVecesNumeros(&dim, matriz1, &consulta);
    MPI_Send( 
    &mas,  
    1, 
    MPI_INT, 
    0,
    0 , 
    MPI_COMM_WORLD);
}


 MPI_Finalize();
 return 0;
}

// esta funcion muestra la matriz generada
void mostrar_matriz(int *dim, int **matriz){
 int i,j;
 for(i=0;i<*dim;i++){
  for(j=0;j<*dim;j++){
   printf("%d\t",matriz[i][j]);
  }
  printf("\n");
 }
}


// esta funcion para llenar la matriz al azar
void llenar_Matriz(int *dim, int **matriz){
 int i,j;
 for(i=0;i<*dim;i++){
  for(j=0;j<*dim;j++){
    matriz[i][j]=  rand() % (MAX - MIN + 1) + MIN;;
  }
 }
}

int buscarVecesNumeros(int *dim, int **matriz, int *consulta){
    int contador = 0;
    for(int i = 0; i < *dim; i++) {
        for(int j = 0; j < *dim; j++) {
            if (*consulta == matriz[i][j]){
                contador++;
            }            
        }
    }
    return contador;
}

// funciones de entrada para mpi

void entradas(int proceso, int nro_procesos, int  *dim, int *consulta)
{
    if (proceso == 0)
    {
    printf("Ingrese la dimension de la matriz: \n");
    scanf("%d",dim);
    printf("Ingrese el numero a buscar dentro de la matriz: \n");
    scanf("%d",consulta);
    }
    MPI_Bcast(dim, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(consulta, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

