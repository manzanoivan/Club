#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

typedef int Costo;
typedef pair<int, int> Arista;
const Costo INF = 1 << 30;
typedef pair<Costo, int> CostoNodo;
typedef pair<Costo, Arista> Ponderada;

struct GrafoPonderado{
	int n; bool bi;
	vector< CostoNodo > padres;
	vector<vector< CostoNodo >> ady;
	GrafoPonderado( int N, bool B = true ) : n(N), bi(B), ady(N), padres( N, CostoNodo( -1, -1 ) ){}

	void AgegarArista( int u, int v, Costo c ){
		if( bi ) ady[v].push_back( CostoNodo( c, u ) );
		ady[u].push_back( CostoNodo(c, v) );
	}

	vector<Costo> Dijkstra( int s ){
		vector<Costo> dist( n, INF );
		priority_queue<CostoNodo> pq;
		pq.push( CostoNodo( 0, s ) ); dist[s] = 0;
		while( !pq.empty() ){
			Costo p = -pq.top().first;
			int u = pq.top().second; pq.pop();
			if( dist[u] < p ) continue;
			for( CostoNodo arista : ady[u] ){
				int v = arista.second;
				p = dist[u] + arista.first;
				if( p < dist[v] ){
					dist[v] = p, pq.push( CostoNodo( -p, v ) );
					padres[ v ] = CostoNodo( arista.first, u );
				}
			}
		}
		return dist;
	}
};

const int MAXN = 1007;
vector<int> Lista;
int memo[MAXN][MAXN];
int ARISTAS;

void limpia(){
	for( int i = 0; i < MAXN; i++ ){
		for( int j = 0; j < MAXN; j++ ){
			memo[i][j] = -1;
		}
	}
}

int dp( int pos, int p ){
	if( pos == ARISTAS ) return 0;
	if( memo[pos][p] != -1 ) return memo[pos][p];

	int temp = -INF;
	if( p - Lista[pos] >= 0 ) temp = dp( pos + 1, p - Lista[pos] ) + 2*Lista[pos];
	return memo[pos][p] = max( temp, dp( pos + 1, p ) + Lista[pos] );
}


int main(){
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	int casos, nodos, aristas, P, x, y, peso, actual;
	cin >> casos;
	while( casos-- ){
		cin >> nodos >> aristas >> P;
		GrafoPonderado grafo( nodos );

		for( int i = 0; i < aristas; i++ ){
			cin >> x >> y >> peso;
			grafo.AgegarArista( --x, --y, peso );
		}
		cin >> x >> y;
		--x, --y;

		vector<int> dist = grafo.Dijkstra( x );

		/*
		for( int i = 0; i < grafo.padres.size(); i++ ){
			cout << i <<" " << grafo.padres[i].first<< " " << grafo.padres[i].second << endl;
		}
		*/

		Lista.clear();

		actual = y;
		while( grafo.padres[actual].second != -1 ){
			Lista.push_back( grafo.padres[actual].first );
			actual = grafo.padres[actual].second;
		}

		ARISTAS = Lista.size();
		limpia();
		int temp = dp( 0, P );
		cout << temp - dist[y] << ' ' << temp << '\n';
	}
	return 0;
}