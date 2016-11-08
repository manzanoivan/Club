#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <sstream>
#include <algorithm>
#include <climits>
using namespace std;

// Definiciones iniciales.

typedef int Flujo; // Ajustable.

typedef vector<int> Lista;
typedef pair<int, int> Par;
typedef vector<Flujo> Flujo1D;
typedef vector<Flujo1D> Flujo2D;

const Flujo FINF = 1 << 30;


// FLUJO MAXIMO
// Nodos indexados de 0 a n - 1.

struct GrafoFlujo {

	int n; vector<Lista> aristas;
	Flujo2D cap, flujo; Lista padre, dist;
	
	GrafoFlujo(int N) : dist(N), padre(N), aristas(N),
		cap(N, Flujo1D(N)), flujo(N, Flujo1D(N)), n(N) {}

	void AgregarArista(int u, int v, Flujo c) {
		//flujo[v][u] += c; // Solo dirigidas!
		cap[u][v] += c, cap[v][u] += c;
		aristas[u].push_back(v);
		aristas[v].push_back(u);
	}

	// Flujo maximo mediante Dinic O(V^2E).

	Flujo FlujoBloqueante(int u, int t, Flujo f) {
		if (u == t) return f; Flujo fluido = 0;
		for (int i = 0; i < aristas[u].size(); ++i) {
			if (fluido == f) break; int v = aristas[u][i];
			if (dist[u] + 1 > dist[v]) continue;
			Flujo fv = FlujoBloqueante(v, t,
				min(f - fluido, cap[u][v] - flujo[u][v]));
			flujo[u][v] += fv, fluido += fv;
			flujo[v][u] -= fv;
		}
		return fluido;
	}

	Flujo Dinic(int s, int t) {
		Flujo flujo_maximo = dist[t] = 0;
		while (dist[t] < INT_MAX) {
			fill(dist.begin(), dist.end(), INT_MAX);
			queue<int> q; q.push(s); dist[s] = 0;
			while (!q.empty()) {
				int u = q.front(); q.pop();
				for (int i = 0; i < aristas[u].size(); ++i) {
					int v = aristas[u][i];
					if (flujo[u][v] == cap[u][v] ||
						dist[v] <= dist[u] + 1) continue;
					dist[v] = dist[u] + 1, q.push(v);
				}
			}
			if (dist[t] < INT_MAX) flujo_maximo +=
				FlujoBloqueante(s, t, FINF);
		}
		return flujo_maximo;
	}
};

int main(){
	int nodos;
	string cad;
	while( getline( cin, cad ) ){
		vector<string> palabras;
		
		for( int i = 0, j = 0; i < cad.size(); i++ ){
			if( cad[i] == ' ' || i+1 == cad.size()){
				palabras.push_back( cad.substr( j, i-j+1 ) );
				i++;
				j = i;
			}
		}

		int minimo = FINF;
		nodos = palabras.size();

		for( int i = 1; i < nodos; i++ ){
			GrafoFlujo grafo( nodos );

			for( int k = 0; k < nodos; k++ ){
				for( int l = k+1; l < nodos; l++ ){
					grafo.AgregarArista( k, l, palabras[k][l] - '0' );
				}
			}

			int flujo = grafo.Dinic( 0, i );
			minimo = min( minimo, flujo );

		}
		cout << minimo << '\n';
	}
	return 0;
}