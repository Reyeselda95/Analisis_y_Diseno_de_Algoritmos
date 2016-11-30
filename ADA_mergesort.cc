/* 
Fichero fuente (incompleto) para realizar la práctica 5 de ADA
El alumno debe implementar la función mergesort haciendo uso de los métodos:
"splice" y "merge" de la "STL list" 
*/

#include <iostream>
#include <list>
#include <stdlib.h>
#include <math.h>
#include <sys/resource.h>

using namespace std;

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

list<int> create_list( int len) {
	list<int> l;
	for( int i = 0; i < len; i++ )
		l.push_front(rand());

	return l;
}


bool is_sorted( list<int> & l ) {

	list<int>::iterator n = l.begin();
	list<int>::iterator i = n++;

	while( n != l.end() ) {
		if( *i > *n )
			return false;
		i = n;
		n++;
	}
	return true;
}

int main() {
long double t1, elapsed_time;

        cout << "Size \t Elapsed time"<< endl;  
        for ( unsigned exp=12; exp<=22; exp++){ 
		const unsigned long list_size = powl(2,exp);
		list<int> l = create_list(list_size);
		t1=tiempo();
		mergesort(l);
		//l.sort();
		elapsed_time=tiempo()-t1;
	
		cout << list_size << " \t " <<  elapsed_time<<endl;
	
		//some checks:
		if( l.size() != list_size ) {
			cout << "ERROR: I've lost some elements" << endl;
			abort();
		}

		if( !is_sorted(l) ) {
			cout << "ERROR: list not sorted" << endl;
			abort();
		}

	}
	cout << "Everything is OK" << endl;
	return 0;
}


