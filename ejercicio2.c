#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include "mpi.h"

#define MIN 1
#define MAX 20

void llenar_Matriz(int *dim, int **matriz);
void mostrar_matriz(int *dim, int **matriz);

// mostrar diagonales
void mostrarDiagonalP(int *dim, int **matriz);
void mostrarDiagonalS(int *dim, int **matriz);
// suma de diagonales
int sumarDiagonalP(int *dim, int **matriz);
int sumarDiagonalS(int *dim, int **matriz);
// mayor de las diagonales
int mayorDiagonalP(int *dim, int **matriz);
int mayorDiagonalS(int *dim, int **matriz);

// comparar diagonales

void comparardiagonales(int vdp , int vds);
void comparargrandeD(int vdp , int vds);

void entradas(int proceso, int nro_procesos, int  *dim);

int main(){
srand(time(NULL));
int dim;
int sumdp , sumds;
int proceso, nros_procesos;
int auxx[4];
int arr[4];
int mayordp , mayords;

MPI_Init(NULL, NULL);
MPI_Comm_rank(MPI_COMM_WORLD, &proceso);
MPI_Comm_size(MPI_COMM_WORLD, &nros_procesos);
entradas(proceso, nros_procesos, &dim);


 int **matriz1 = (int **)malloc(dim * sizeof(int *));
 for (int i = 0; i < dim; i++) {
    matriz1[i] = (int *)malloc(dim * sizeof(int));
}

if (proceso == 0)
{
    for (int  i = 3; i < nros_procesos; i++){
        MPI_Recv( 
        &auxx , 
        4, 
        MPI_INT , 
        i, 
        0, 
        MPI_COMM_WORLD , 
        MPI_STATUS_IGNORE);
        //arr= auxx;
    }
    printf("\n La suma de la diagonal principal es: %d \n", auxx[0]);
    printf("\n La suma de la diagonal Secundaria es: %d \n ", auxx[1]);
    printf("\n El valor mas alto de la diagonal principal es: %d \n", auxx[2]);
    printf("\n El valor mas alto de la diagonal secundaria es: %d \n", auxx[3]);
    
} 
if (proceso==3)
{       
    llenar_Matriz(&dim,matriz1);
    mostrarDiagonalP(&dim, matriz1);
    mostrarDiagonalS(&dim, matriz1);
    sumdp = sumarDiagonalP(&dim, matriz1);    
    sumds = sumarDiagonalS(&dim, matriz1); 
    mayordp = mayorDiagonalP(&dim, matriz1);
    mayords = mayorDiagonalS(&dim, matriz1);
    //mostrarDiagonalP(&dim, matriz1);
    auxx[0] = sumdp;
    auxx[1] = sumds;
    auxx[2] = mayordp;
    auxx[3] = mayords;
    MPI_Send( &auxx,4, MPI_INT, 0,0 , MPI_COMM_WORLD);   
    MPI_Send( &auxx,4, MPI_INT, 2, 0 , MPI_COMM_WORLD);   
}
if (proceso == 2){
    MPI_Recv( &auxx , 4, MPI_INT ,  3, 0, MPI_COMM_WORLD , MPI_STATUS_IGNORE);
    comparardiagonales(auxx[2], auxx[3]);  
    comparargrandeD (auxx[0], auxx[1]); 
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
printf("\nLa matriz generada es la siguiente \n");
mostrar_matriz(dim, matriz);
}

void mostrarDiagonalP(int *dim, int **matriz){
    printf("\nPrincipal\n -  ");
    for(int i = 0; i < *dim; i++) {
        printf("%d",matriz[i][i]);
        printf(" - ");
    }
}
void mostrarDiagonalS(int *dim, int **matriz){
    printf("\nSecundaria\n - ");
    for(int i = *dim -1 , j = 0 ; i >= 0 && j < *dim; --i, ++j) {
        printf("%d",matriz[i][j]);
        printf(" - ");
    }
    printf("\n");
}

// sumas de diagonales


int sumarDiagonalP(int *dim, int **matriz){
    int sumdp = 0 ;
    for(int i = 0; i < *dim; i++) {
       sumdp= sumdp + matriz[i][i] ;
        
    }
    return sumdp;
}
int sumarDiagonalS(int *dim, int **matriz){
    int sumds = 0 ;
    for(int i = *dim -1 , j = 0 ; i >= 0 && j < *dim; --i, ++j) {
        sumds= sumds+ matriz[i][j] ;
    }
    return sumds;
}


// nuumero m[as grande de las diagonales


int mayorDiagonalP(int *dim, int **matriz){
    int mayor = 0 ;
    for(int i = 0; i < *dim; i++) {
       if(matriz[i][i] >= mayor){
           mayor = matriz[i][i];
       }
        
    }
    return mayor;
}
int mayorDiagonalS(int *dim, int **matriz){
    int mayor = 0;
    for(int i = *dim -1 , j = 0 ; i >= 0 && j < *dim; --i, ++j) {
        
        if(matriz[i][j] >= mayor){
            
            mayor = matriz[i][j];
        }
    }
    return mayor;
}


// comparar cual tiene el valor mas alto de ambas diagonales

void comparardiagonales(int vdp , int vds){
    if (vdp > vds){
        printf("\n La diagonal principal tiene el valor más alto, con el valor :%d \n", vdp);
    }else{
        if (vds > vdp){
            printf("\nLa diagonal secundaria tiene el valor más alto, con el valor :%d \n", vds);
        }else{
            if (vds==vdp) {
               printf("\nAmbas diagonales cunetan con el valor más alto, con el valor :%d \n", vdp);
            }
        }      
    }
    
}

void comparargrandeD(int vdp , int vds){
    if (vdp > vds){
        printf("\n La diagonal principal es mas grande, con un valor valor de :%d \n", vdp);
    }else{
        if (vds > vdp){
            printf("\nLa diagonal secundaria es mas grande, con un valor valor de :%d \n", vds);
        }else{
            if (vds==vdp) {
               printf("\n Ambas diagonales son igual de grandes con una suma igual a %d \n", vdp);
            }
        }      
    }
    
}

void entradas(int proceso, int nro_procesos, int  *dim)
{
    if (proceso == 0)
    {
    printf("Ingrese la dimension de la matriz: \n");
    scanf("%d",dim);
    }
    MPI_Bcast(dim, 1, MPI_INT, 0, MPI_COMM_WORLD);
}
