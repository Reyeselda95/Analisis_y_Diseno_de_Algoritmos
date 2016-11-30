#include <sys/resource.h>

using namespace std;

/*****************************************************
Nombre funcion: tiempo(void)
Proposito: Calcular el tiempo de CPU de este proceso
(y sólo de este proceso)
Parametros de entrada: nada
Devuelve: El tiempo empleado en segundos
******************************************************/
long double
tiempo(void){
 struct rusage usage;
 getrusage(RUSAGE_SELF,&usage);
 return( (long double) usage.ru_utime.tv_sec +
	 (long double) usage.ru_utime.tv_usec/1e6 
	 /* + (long double) usage.ru_stime.tv_sec +
         (long double) usage.ru_stime.tv_usec/1e6 */  
       );
}

/******************************************************************
quicksortCentral(int * v, long pi, long pf)

Algoritmo de ordenación Quicksort tomando como pivote el
elemento del vector que ocupa la posición central
*******************************************************************/
void 
quicksortCentral(int * v, long pi, long pf){
long i,j;
int pivote,aux; 
  if (pi<pf){
	i=pi; j=pf;
	pivote=v[(i+j)/2];
	do{
		while (v[i]<pivote) i++;
		while (v[j]>pivote) j--;
		if (i<=j) {
		  aux=v[i]; v[i]=v[j]; v[j]=aux;
		  i++; j--;
		}
	}while (i<=j);
	if (pi<j) quicksortCentral(v,pi,j);
	if (i<pf) quicksortCentral(v,i,pf);
  }
}

