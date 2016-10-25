#include <iostream>
#include <climits>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

typedef int Flujo;
typedef vector<int> Lista;
typedef pair<int, int> Par;
typedef vector<Flujo> Flujo1D;
typedef vector<Flujo1D> Flujo2D;
const Flujo FINF = 1 << 30;

struct GrafoFlujo{
	int n; vector<Lista> aristas;
	Flujo2D cap, flujo; Lista padre, dist;

	GrafoFlujo( int N ) : dist( N ), padre(N), aristas(N), cap( N, Flujo1D(N) ), flujo( N, Flujo1D(N) ), n(N){}

	void AgregarArista( int u, int v, Flujo c ){
		flujo[v][u] += c;
		cap[u][v] += c, cap[v][u] += c;
		aristas[u].push_back( v );
		aristas[v].push_back( u );
	}

	Flujo FlujoBloqueante( int u, int t, Flujo f ){
		if( u == t ) return f; Flujo fluido = 0;
		for( int i = 0; i < aristas[u].size(); i++ ){
			if( fluido == f ) break; int v = aristas[u][i];
			if( dist[u] + 1 > dist[v] ) continue;
			Flujo fv = FlujoBloqueante( v, t, min( f - fluido, cap[u][v] - flujo[u][v] ) );
			flujo[u][v] += fv, fluido += fv;
			flujo[v][u] -= fv;
		}
		return fluido;
	}

	Flujo Dinic( int s, int t ){
		Flujo flujo_maximo = dist[t] = 0;
		while( dist[t] < INT_MAX){
			fill( dist.begin(), dist.end(), INT_MAX );
			queue<int> q; q.push(s); dist[s] = 0;
			while( !q.empty() ){
				int u = q.front(); q.pop();
				for( int i = 0; i < aristas[u].size(); i++ ){
					int v = aristas[u][i];
					if( flujo[u][v] == cap[u][v] || dist[v] <= dist[u] + 1 ) continue;
					dist[v] = dist[u] + 1, q.push(v);
				}
			}

			if( dist[t] < INT_MAX ) flujo_maximo += FlujoBloqueante( s, t, FINF );
		}
		return flujo_maximo;
	}
};

int main(){
	char car;
	int N, M, G, x, y;
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	while( 1 ){
		cin >> N >> M >> G;
		if( !N && !M && !G ) break;

		Flujo2D matches(N);

		for( int i = 0; i+1 < N; i++){
			for( int j = i+1; j < N; j++ ){
				for( int k = 0; k < M; k++ ){
					matches[i].push_back( j );
				}
			}
		}

		int nodos = (N*(N-1))/2;
		nodos *= 3*M;
		nodos += N;
		nodos += 2;

		int source = nodos - 2, sink = nodos - 1;
		GrafoFlujo grafo( nodos );
		Lista puntos( N, 0 );

		for( int i = 0; i < G; i++ ){
			cin >> x >> car >> y;
			if( car == '=' ){
				puntos[x]++;
				puntos[y]++;
			}
			else{
				puntos[y] += 2;
			}
			if( x > y ) swap( x, y );
			matches[x].erase( lower_bound( matches[x].begin(), matches[x].end(), y ) );
		}

		int cont = N, flujo_esperado = 0;
		for( int i = 0; i < matches[0].size(); i++ ){
			puntos[0]+= 2;
		}

		for( int i = 1 ; i < N; i++ ){
			for( int j = 0; j < matches[i].size(); j++){
				grafo.AgregarArista( source, cont, 2 );
				grafo.AgregarArista( cont, cont+1, 2 );
				grafo.AgregarArista( cont, cont+2, 2 );
				grafo.AgregarArista( cont+1, i, 2 );
				grafo. AgregarArista( cont+2, matches[i][j], 2 );
				flujo_esperado += 2;
				cont += 3;
			}
		}

		grafo.AgregarArista( 0, sink, FINF );
		flujo_esperado -= grafo.Dinic( source, sink );
		
		
		for( int i = 0; i < nodos; i++ ){
			if( grafo.flujo[source][i] ){
				grafo.cap[source][i] = 0;
			}
		}

		int maximo = grafo.flujo[0][sink] + puntos[0] - 1;
		for( int i = 1; i < N; i++ ){
			grafo.AgregarArista( i, sink, maximo-puntos[i] );
		}

		if( flujo_esperado == grafo.Dinic( source, sink ) ){
			maximo++;
			bool band = true;
			for( int i = 1; i < N; i++ ){
				puntos[i] += grafo.flujo[i][sink];
				if( maximo <= puntos[i] ){
					band = false;
				}
			}

			if( band ) cout << "Y\n";
			else cout << "N\n";
			
		}
		else{
			cout << "N\n";
		}
	}

	return 0;
}