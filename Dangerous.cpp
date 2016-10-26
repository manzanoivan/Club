#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#define mp make_pair
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

	GrafoFlujo( int N ) : dist(N), padre(N), aristas(N), cap( N, Flujo1D(N) ), flujo( N, Flujo1D(N) ), n(N){}

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
			queue<int> q; q.push( s ); dist[s] = 0;

			while( !q.empty() ){
				int u = q.front(); q.pop();
				for( int i = 0; i < aristas[u].size(); i++ ){
					int v = aristas[u][i];
					if( flujo[u][v] == cap[u][v] || dist[v] <= dist[u] + 1) continue;
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
	int nodos, source, sink, N, M, x, y, K, peso, caso = 1;
	while( 1 ){
		cin  >> N;
		if( !N ) break;

		vector< pair< int, Par > > aristas;
		nodos = (N+5)*2;
		source = 1; sink = 2*(N+1);

		cin >> M;
		for( int i = 0; i < M; i++ ){
			cin >> x >> y >> peso;
			if( x > y ) swap( x, y );
			aristas.push_back( mp( peso, Par( x, y ) ) );
		}

		cin >> K;

		int ini = 0, fin = 100000, mid, res = FINF;
		while( ini < fin ){
			mid = ( ini + fin ) / 2;
			GrafoFlujo grafo( nodos );
			
			//cout << "Mid: " << mid<< " " << source << " " << sink << endl; 
			for( int i = 0; i < M; i++ ){
				if( aristas[i].first <= mid ){
					x = aristas[i].second.first;
					y = aristas[i].second.second;
					//cout << "Arista: " << x*2 + 1 << " -> " << y*2 << endl;
					grafo.AgregarArista( x*2 + 1, y*2, 1 );
				}
			}

			for(int i = 1; i < N+1; i++  ){
				//cout << "Arista: " << i*2 << " -> " << i*2+1 << endl;
				grafo.AgregarArista( i*2, i*2 + 1, 1 );
			}


			int flujo = grafo.Dinic( source, sink );
			//cout << "Flujo " << flujo << endl;
			if( flujo >= K ){
				fin = mid;
				res = mid;
			}
			else{
				ini = mid+1;
			}
		}
		cout << "Case " << caso++ << ": ";
		if( res == FINF ) cout << "no solution\n";
		else cout << res << '\n';


	}
	return 0;
}