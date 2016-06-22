#include <iostream>
#include <vector>
using namespace std;

//Nodos indexados 0 -> N-1

struct Grafo{
	int n;
	vector < vector< int > > aristas;
	Grafo( int N ): n( N ), aristas( n, vector<int>() ){}

	void AgregarArista( int u, int v ){
		aristas[u].push_back( v );
	}
};

int main(){
	
	return 0;
}