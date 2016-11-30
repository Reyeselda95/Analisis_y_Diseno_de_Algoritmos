#include <iostream>
#include<vector>
#include <fstream>
#include <stdlib.h>
#include <sys/resource.h>
#include <queue>

using namespace std;

struct Nodo
{
	int valor;
	int camino;
	int row;
	int column;
	bool operator<(Nodo n2) cons
	{
		return valor<n2.valor;
	}
};


void comprobarCam(vector<vector<int> > lab, Nodo &nodo, int row, int column)
{
	if(nodo.row+1==row-1&&nodo.column=column-1)
		return;
	if(lab[nodo.row+1][nodo.column]==1)
		nodo.valor++;
	if(lab[nodo.row][nodo.column+1]==1)
		nodo.valor++;
	if(lab[nodo.row+1][nodo.column+1]==1)
		nodo.valor++;
	nodo.valor+=nodo.camino;
}

int ramificacionypoda(vector<vector<int> > lab, int row, int column, int &iteraciones)
{
	Nodo nodo;
	nodo.row=0;
	nodo.column=0;
	nodo.camino=1;
	priority_queue<Nodo> cola;
	int iteraciones=0;
	if(lab[0][0]==0||lab[row-1][column-1]==0
		return 0;
	while(true)
	{
		if(nodo.row<row-1&&nodo.column<column-1&&lab[nodo.row++][nodo.column++]!=0)
		{
			Nodo n;
			n.row=nodo.row++;
			n.column=nodo.column++;
			comprobarCam(lab,n,row,column);
			cola.push(n);
		}
		if(nodo.row<row-1&&lab[nodo.row++][nodo.column]!=0)
		{
			Nodo n;
			n.row=nodo.row++;
			comprobarCam(lab,n,row,column);
			cola.push(n);
		}
		if(nodo.column<column-1&&lab[nodo.row][nodo.column++]!=0)
		{
			Nodo n;
			n.column=nodo.column++;
			comprobarCam(lab,n,row,column);
			cola.push(n);
		}
		nodo=cola.top();
		cola.pop();
	}
	return nodo.camino;
}

int main(int argc,char** argv){
  if(argc==2){
    ifstream ifs(argv[1]);
    if(!ifs){
      cout<<"No se ha podido abrir el archivo\n";
      return -1;
    }
    while(!ifs.eof)
    {
	    int row,colum;
	    ifs>>row>>colum;
	    vector<vector<int> > lab(row,vector<int>(colum));
	    for(int i=0;i<row;i++){
	      for(int j=0;j<colum;j++){
	    ifs>>lab[i][j];
	      }
	    }
	    
	    int iteraciones;
	   	clock_t start,end;
	   	start=clock();
	    int lenght=ramificacionypoda(lab,row,colum,&iteraciones);
	    end=clock();
   		cout<<"Maze = "<<i<<" Lenght best path = "<<lenght<< ". Iterations = "<<iteraciones<<" Time = "<<(end-start);
   		i++;
	}
  }
  else
    cout<<"Tienes que introducir el nombre del archivo\n";
  return 0;
}
