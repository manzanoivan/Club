#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <climits>
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

	GrafoFlujo( int N ) : dist(N), padre(N), aristas(N), cap(N, Flujo1D(N)), flujo( N, Flujo1D(N) ), n(N){}

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
		while( dist[t] < INT_MAX ){
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
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	int casos, rows, cols, aux, source, sink, acum;
	cin >> casos;
	for( int cases = 1; cases <= casos; cases++ ){
		if( cases > 1 ) cout << '\n';
		cout << "Matrix " << cases << '\n';
		
		cin >> rows >> cols;
		GrafoFlujo grafo( rows + cols + 2 );
		
		source = rows + cols;
		sink = source+1;
		
		acum = 0;
		for( int i = 0; i < rows; i++ ){
			cin >> aux;
			grafo.AgregarArista( source, i, aux - acum - cols );
			acum += aux - acum;
		}

		acum = 0;
		for( int i = 0; i < cols; i++ ){
			cin >> aux;
			grafo.AgregarArista( rows+i, sink, aux - acum - rows );
			acum += aux - acum;
		}

		for( int i = 0; i < rows; i++ ){
			for( int j = 0; j < cols; j++ ){
				grafo.AgregarArista( i, rows+j, 19 );
			}
		}

		grafo.Dinic( source, sink );

		for( int i = 0; i < rows; i++ ){
			for( int j = 0; j < cols; j++ ){
				if( j ) cout << ' ';
				cout << grafo.flujo[i][j+rows] + 1;
			}
			cout << '\n';
		}

	}

	return 0;
}