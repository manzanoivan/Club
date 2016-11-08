#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <climits>
using namespace std;

typedef double Flujo;
typedef vector<int> Lista;
typedef pair<int, int> Par;
typedef vector<Flujo> Flujo1D;
typedef vector<Flujo1D> Flujo2D;
const Flujo FINF = 1 << 30;

typedef double Costo;

typedef vector<Costo> Costo1D;
typedef vector<Costo1D> Costo2D;
typedef pair<Costo, int> CostoNodo;
typedef pair<Flujo, Costo> FlujoCosto;
const double ERROR = 1e-9;
const Costo CINF = 1 << 30;

bool Igual( double a, double b ){
	return fabs( a - b ) < ERROR;
}

struct GrafoFlujoCosto{
	struct AristaFlujo{
		int dst; AristaFlujo* residual;
		Flujo cap, flujo;
		Costo peso, npeso;
		AristaFlujo( int d, Flujo f, Flujo c ) : dst( d ), flujo(f), cap(c){}
		Costo AumentarFlujo(Flujo f){
			residual -> flujo -= f;
			this -> flujo += f;
			return peso*f;
		}
	};

	int n; vector<Par> prv; Lista dist;
	vector< vector<AristaFlujo*> > aristas;
	GrafoFlujoCosto( int N ) : n(N), aristas(N), prv(N), dist(N){}

	~GrafoFlujoCosto(){
		for( int i = 0; i < n; i++ )
			for( int j = 0; j < aristas[i].size(); j++ )
				delete aristas[i][j];
	}

	void AgregarArista( int u, int v, Flujo c, Costo p = 0 ){
		AristaFlujo* uv = new AristaFlujo( v, 0, c );
		AristaFlujo* vu = new AristaFlujo( u, c, c );
		uv->residual = vu, vu->residual = uv;
		uv->peso = uv->npeso = p;
		vu->peso = vu->npeso = -p;
		aristas[u].push_back( uv );
		aristas[v].push_back( vu );
	}

	Flujo FlujoBloqueante( int u, int t, Flujo f ){
		if( u == t ) return f; Flujo fluido = 0;
		for( int i = 0; i < aristas[u].size(); i++ ){
			if( fluido == f ) break;
			AristaFlujo* v = aristas[u][i];
			if( dist[u] + 1 == dist[v->dst]){
				Flujo fv = FlujoBloqueante( v->dst, t, min( f-fluido, v->cap - v->flujo ) );
				v->AumentarFlujo( fv ), fluido += fv;
			}
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
					AristaFlujo* v = aristas[u][i];
					if( dist[v->dst] < INT_MAX )continue;
					if( v->flujo == v->cap )continue;
					dist[v->dst] = dist[u] + 1;
					q.push( v->dst );
				}
			}
			if( dist[t] < INT_MAX )
				flujo_maximo += FlujoBloqueante( s, t, FINF );
		}
		return flujo_maximo;
	}

	void RecalcularCosto( const Costo1D& pi ){
		for( int u = 0; u < n; u++ ){
			for( int i = 0; i < aristas[u].size(); i++ ){
				AristaFlujo* v = aristas[u][i];
				v->npeso = v->npeso + pi[u] - pi[v->dst];
			}
		}
	}

	FlujoCosto ActualizarFlujo( int u, Flujo f ){
		int p = prv[u].first, i = prv[u].second;
		if( p == -1 ) return FlujoCosto( f, 0 );
		AristaFlujo* pu = aristas[p][i];
		FlujoCosto res = ActualizarFlujo( p, min( f, pu->cap - pu->flujo ) );
		res.second += pu->AumentarFlujo( res.first );
		return res;
	}

	FlujoCosto AumentarFlujo( int s, int t, Flujo f ){
		Costo1D dist( n, CINF );
		fill( prv.begin(), prv.end(), Par( -1, -1 ) );
		priority_queue <CostoNodo, vector<CostoNodo>, greater<CostoNodo> > pq;
		pq.push( FlujoCosto( 0, s ) ); dist[s] = 0;
		while( !pq.empty() ){
			int u = pq.top().second;
			Costo p = pq.top().first;
			pq.pop();
			if( !Igual( dist[u], p ) ) continue;
			for( int i = 0; i < aristas[u].size(); i++ ){
				AristaFlujo* v = aristas[u][i];
				if( v->flujo == v->cap ) continue;
				Costo ndist = dist[u] + v->npeso;
				if( !Igual( ndist, dist[v->dst] ) && ndist < dist[ v->dst ]){
					dist[ v->dst ] = dist[u] + v->npeso;
					pq.push( CostoNodo( ndist, v->dst ) );
					prv[ v->dst ].second = i;
					prv[v->dst].first = u;
				}
			}
		}
		if( Igual( dist[t], CINF ) ) return FlujoCosto( 0, 0 );
		RecalcularCosto( dist );
		return ActualizarFlujo( t, f );
	}

	FlujoCosto FlujoCostoMin( int s, int t, Flujo k = FINF ){
		Costo1D dist( n, CINF ); dist[s] = 0;
		for( int i = 0; i < n; i++ ){
			for( int u = 0; u < n; u++ ){
				if( Igual( dist[u], CINF ) ) continue;
				for( int j = 0; j < aristas[u].size(); j++ ){
					AristaFlujo* v = aristas[u][j];
					if( v->flujo < v->cap )
						dist[v->dst] = min( dist[v->dst], dist[u]+ v->npeso );
				}
			}
		}

		RecalcularCosto( dist );
		FlujoCosto flujo_costo( 0, 0 );
		while( flujo_costo.first < k ){
			FlujoCosto fc = AumentarFlujo( s, t, k - flujo_costo.first );
			flujo_costo.second += fc.second;
			flujo_costo.first += fc.first;
			if( !fc.first ) break;
		}
		return flujo_costo;
	}
};

int main(){
	cin.tie(0);
	ios_base::sync_with_stdio( 0 );
	double prestigio;
	int N, M, mujeres, id;
	
	cin >> N >> M;

	GrafoFlujoCosto grafo( N+M+2 );
	int source = N+M, sink = source+1;
	
	for( int i = 0; i < N; i++ ){
		cin >> prestigio >> mujeres;
		grafo.AgregarArista( source, i, log10( prestigio )/10.0, 0 );
		for( int j = 0; j < mujeres; j++ ){
			cin >> id;
			grafo.AgregarArista( i, N+id-1, 1, 0 );
		}
	}

	for( int i = 0; i < M; i++ ){
		grafo.AgregarArista( i+N, sink, 1, 0 );
	}

	cout << fixed << setprecision(0) << floor(grafo.Dinic( source, sink )*10.0) + 1 << '\n';

	return 0;
}