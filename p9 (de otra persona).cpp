#include <iostream>
#include <sys/resource.h>
#include <stdlib.h>
#include <fstream>
#include <queue>
#include <stack>
#include <vector>


using namespace std;

unsigned n, m;
unsigned n_nodos;
int **laberinto;

class casilla
{
	public:
		unsigned i, j;
		bool operator==(const casilla& c)
		{
			return (this->i == c.i && this->j == c.j);
		}
};

//Comprueba si la casilla actual es la esquina inferior derecha
//del tablero, la salida.
bool heLlegado(casilla actual)
{	
	if(actual.i == n-1 && actual.j == m-1)
		return true;
	else
		return false;
}

//Vamos a codificar los movimientos de tal forma que la diagonal sea 1, derecha 2 y hacia abajo 3.
casilla getCasillaActual(vector<int> aux)
{
	casilla actual;
	actual.i=0;
	actual.j=0;
	for(unsigned i=0; i<aux.size(); i++)
	{
		switch(aux[i])
		{
			case 1:
				actual.i++;
				actual.j++;
				break;
			case 2:
				actual.j++;
				break;
			case 3:
				actual.i++;
		}
	}
	
	return actual;
}

//Comprueba si la casilla actual está en una posicion valida del laberinto
bool factible(vector<int> mov)
{
	casilla actual = getCasillaActual(mov);
	if((actual.i < n && actual.j < m) && 
		laberinto[actual.i][actual.j] == 1)
		return true;
	else
		return false;
}

class Node
{
	private:
		vector<int> movimientos;
		//Numero de nodo
		unsigned k;
		
	public: 
		Node()
		{
			movimientos = vector<int>();
		}
		
		vector<int> getMovimientos()
		{
			return movimientos;
		}
		
		void initialNode()
		{
			this->k = 1;
			n_nodos = 1;
		}
		
		unsigned optimistic_b() const
		{
			unsigned length=movimientos.size();
			casilla actual = getCasillaActual(movimientos); 
			for(unsigned i=movimientos.size(); i<n+m && !heLlegado(actual);i++)
			{
				if(actual.i < n && actual.j < m)
				{
					actual.i++;
					actual.j++;
				}
				else if(actual.i < n)
				{ actual.i++; }
				else
				{actual.j++;}
			
				length++;
			}
			
			return length;
		}
	
		unsigned pessimistic_b() const
		{
			unsigned length=movimientos.size();
			casilla actual = getCasillaActual(movimientos); 
			bool casillaMovida;
			for(unsigned i=movimientos.size(); i<n+m && !heLlegado(actual) && casillaMovida; i++)
			{
				casillaMovida = false;
				if(actual.i<n && laberinto[actual.i+1][actual.j] == 1)
				{
					actual.i++;
					length++;
					casillaMovida = true;
				}
				else if(actual.j<n && laberinto[actual.i][actual.j+1] == 1)
				{
					actual.j++;
					length++;
					casillaMovida = true;
				}
				else if(actual.i<n && actual.j<m && laberinto[actual.i+1][actual.j+1] == 1)
				{
					actual.i++;
					actual.j++;
					length++;
					casillaMovida = true;
				}
			}
		
		
			if(heLlegado(actual))
				return length;
			else
				return 4294967295;
		}
		
		//expand lo que nos va a hacer va a ser darnos los posibles nodos a visitar desde
		//el nodo actual, hay que cambiar la k de los nodos
		
		
		vector<Node> expand() const 
		{
			vector<Node> aux;
			Node n1;
			n1.movimientos = this->movimientos;
			
			for(unsigned i=1; i<=3; i++)
			{
				n1.movimientos.push_back(i);
				if(factible(n1.movimientos))
				{
					n_nodos++;
					n1.k = n_nodos;
					aux.push_back(n1);
				}
				
				n1.movimientos.pop_back();
			}
			
			return aux;
		}
		
		bool operator==(const Node& n)
		{
			return getCasillaActual(this->movimientos) == getCasillaActual(n.movimientos);
		}
};

class mycomp {
	public:
		bool operator()(
			const Node& n1,
			const Node& n2
		) const 
		{ return n1.optimistic_b() < n2.optimistic_b(); }
};
 
 typedef priority_queue<Node, vector<Node>, mycomp> myqueue;
 
 typedef struct
 {
	 int length_path;
	 unsigned long iterations;
 } Solucion;

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

void imprimirLaberinto()
{
	cout << "---------------" << endl;
	cout << "-- Laberinto --" << endl;
	cout << "---------------" << endl;
	//imprimimos el laberinto
	for(unsigned i=0; i<n; i++)
	{
		for(unsigned j=0; j<m; j++)
			cout << laberinto[i][j] << " ";
		cout << endl;
	}
}

void imprimirCamino(vector<int> mov)
{
	casilla actual;
	actual.i=0; 
	actual.j=0;
	
	for(unsigned i=0; i<mov.size(); i++)
	{
		switch(mov[i])
		{
			case 1:
				actual.i++;
				actual.j++;
				break;
			case 2:
				actual.j++;
				break;
			case 3:
				actual.i++;
		}
		
		cout << "[" << actual.i << ", " << actual.j << "] ";
	}
	cout << endl;
}

bool existe(stack<Node> q, Node n)
{
	bool existe = false;
	for(unsigned i=0; i<q.size() && !existe; i++)
	{
		Node n2 = q.top();
		if(n2 == n)
			existe = true;
		q.pop();
	}
	
	return existe;
}

Solucion ramificacion_poda()
{
	Node init = Node();
	init.initialNode();
	unsigned best = init.pessimistic_b();
	unsigned mejor_cota_posible = init.optimistic_b();
	unsigned long iterations = 0;
	myqueue q;
	stack<Node> s;
	q.push(init);
	
	while(!q.empty() && best > mejor_cota_posible)
	{
		
		Node n = q.top();
		q.pop();
		
		if(n.getMovimientos().size()+1 < best )
		{
			s.push(n);
			if(heLlegado(getCasillaActual(n.getMovimientos())))
			{
				unsigned size = n.getMovimientos().size();
				if(size < best)
				{
					best = size;
				}
			}
			else
			{
				//Me va a devolver la lista de nodos posibles
				vector<Node> aux = n.expand();
				
				for(unsigned i=0; i<aux.size(); i++)
				{
					if(!existe(s, aux[i]))
					{
						q.push(aux[i]);
					}
				}
			}
		}
		
		iterations+=1;
	}
	
	Solucion aux;
	aux.iterations = iterations;
	aux.length_path = best;
	
	return aux;
}

int main(int argc, char *argv[])
{
	if(argc == 2)
	{
		unsigned short it=0;
		long double tAnterior, tActual,tPrograma;
		
		ifstream f(argv[1]);
	
		if(f.is_open())
		{
			while(!f.eof())
			{
				f >> n >> m;
				
				laberinto = new int*[n];
	
				for(unsigned i=0; i<n; i++)
				{
					laberinto[i] = new int[m];
					for(unsigned j=0; j<m; j++)
					{
						f >> laberinto[i][j];
					}
				}
			
				//imprimirLaberinto();
				
				tAnterior = tiempo();
				Solucion solution = ramificacion_poda();
				tActual = tiempo();
				tPrograma = tActual-tAnterior;
				
				
				
				cout << "Maze " << it << ": Length best path= " << solution.length_path << ". Iterations= " 
					<< solution.iterations << ". Time= " << tPrograma << endl;
				it++;
			}
			
			f.close();
		}
		else
			cout << "ERROR al leer el fichero " << argv[1] << endl;
		
			
		return 0;
	}
	else
	{
		cout << "ERROR: Parametros incorrectos.\n" << endl;
		
		return -1;
	}
}





