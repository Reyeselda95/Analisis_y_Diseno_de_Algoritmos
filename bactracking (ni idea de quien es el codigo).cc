#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

bool **laberinto;

unsigned n, m; //n: anchura laberinto, m: altura laberinto
bool esHoja(vector<char> &sol) {

	int i = 0, j = 0;
	for (int k = 0; k < sol.size(); k++) {
		if (sol[k] == '1') {
			i++;
			j++;

		} else if (sol[k] == '2') {
			i++;
		} else if (sol[k] == '3') {
			j++;
		}
	}
	if (i == n - 1 && j == m - 1) {
		return true;
	}
	return false;
}

int coste(vector<char> &sol) {
	return sol.size();
}

void fi(int i, int j, vector<char> &sol, int &mincost, vector<char> &best_sol) {

	//if (!factible(sol)) {
	//	return;
	//}
	if (!(i < n && j < m && laberinto[i][j]))
		return;

	if (esHoja(sol)) {
		int cost = coste(sol);
		if (cost < mincost) {
			mincost = cost;
			best_sol = sol;
		}
		return;
	}

	sol.push_back('1');
	fi(i+1,j+1,sol, mincost, best_sol);
	sol.pop_back();
	sol.push_back('2');
	fi(i+1,j,sol, mincost, best_sol);
	sol.pop_back();
	sol.push_back('3');
	fi(i,j+1,sol, mincost, best_sol);
	sol.pop_back();

}

int main(int argc, char *argv[]) {

	fstream f(argv[1]);
	f >> n >> m;
	laberinto = new bool *[n];

	vector<char> sol;
	int mincost = 9999;
	int a = 0, b = 0;

	for (unsigned i = 0; i < n; i++) {
		laberinto[i] = new bool[m];
		for (unsigned j = 0; j < m; j++) {
			f >> laberinto[i][j];
		}
	}
	cout << endl;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << laberinto[i][j];
		}
		cout << endl;
	}

	vector<char> best_sol;
	fi(0,0,sol, mincost, best_sol);
	cout << endl;
	cout << "Camino: ";

	for (int k = 0; k < best_sol.size(); k++) {
		if (best_sol[k] == '1') {
			a++;
			b++;
		} else if (best_sol[k] == '2') {
			a++;
		} else {
			b++;
		}
		cout << " (" << a << "," << b << ")";
	}
	cout << endl;
	cout << endl;
	cout << "Longitud: " << best_sol.size() << endl;

}
