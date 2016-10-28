#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <climits>
using namespace std;

const int MAXN = 10000;

typedef int Flujo;
typedef vector<int> Lista;
typedef pair<int, int> Par;
typedef vector<Flujo> Flujo1D;
typedef int Costo;
typedef vector<Flujo1D> Flujo2D;

const Flujo FINF = 1 << 30;

struct GrafoFlujo {

	int n; vector<Lista> aristas;
	Flujo2D cap, flujo; Lista padre, dist;
	
	GrafoFlujo(int N) : dist(N), padre(N), aristas(N),
		cap(N, Flujo1D(N)), flujo(N, Flujo1D(N)), n(N) {}

	void AgregarArista(int u, int v, Flujo c) {
		flujo[v][u] += c; // Solo dirigidas!
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
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	int N, a, b;
	vector<Par> aristas;

	int ini = 0, fin = MAXN, mid, res = 0, upper, lower;

	cin >> N;
	vector< int > sweep;
	for( int i = 0; i < N; i++ ){
		cin >> a >> b;
		aristas.push_back( Par(a, b) );
		sweep.push_back(a);
		sweep.push_back(b);
	}
	sort( sweep.begin(), sweep.end() );
	vector<int>::iterator it = unique (sweep.begin(), sweep.end());
	sweep.resize( distance(sweep.begin(),it) );
	
	int nodos = sweep.size() + N + 2;
	int source = nodos - 2, sink = source + 1;
	cout << "Nodos: " << nodos << endl;
	while( ini <= fin ){
		int aris = 0;
		mid = ( ini + fin ) / 2;
		//cout << mid << endl;
		GrafoFlujo grafo( nodos );

		for( int i = 0; i < N; i++ ){
			aris++;
			grafo.AgregarArista( source, i , mid );
			upper = lower_bound( sweep.begin(), sweep.end(), aristas[i].second ) - sweep.begin();
			lower = lower_bound( sweep.begin(), sweep.end(), aristas[i].first ) - sweep.begin();
			for( int j = lower; j < upper; j++ ){
				aris++;
				grafo.AgregarArista( i, j+N, sweep[j+1]-sweep[j] );
			}
		}

		for( int i = 0; i < sweep.size(); i++ ){
			aris++;
			grafo.AgregarArista( i+N, sink, sweep[i+1]-sweep[i] );
		}
		cout << "Aristas: " << aris << endl;
		int flujo = grafo.Dinic( source, sink );
		//cout << flujo << " " << N*mid<< " ";
		if( flujo == N*mid){
			res = mid;
			ini = mid + 1;
		}
		else{
			fin = mid-1;
		}
	   // cout << ini << " " << fin << endl;
	}

	cout << res*N << '\n';

	return 0;
}