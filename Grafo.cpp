#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
using namespace std;

// Nodos indexados de 0 a n - 1.

typedef vector<int> Lista;

const int INF = 1 << 30;

struct Grafo {

	int n; bool bi;
	vector<Lista> aristas;
	Grafo(int N, bool B = true) : n(N),
		bi(B), aristas(N, Lista()) {}

	void AgregarArista(int u, int v) {
		if (bi) aristas[v].push_back(u);
		aristas[u].push_back(v);
	}

	vector<char> color;

	// Detecta ciclos en una componente conexa bidireccional y direccional.
	// Devuelve true o false si existe un ciclo o no en la componente.

	bool DetectarCiclo(int u,
		int p, bool detected) {
		color[u] = detected? 'N': 'G';

		for (int v : aristas[u]) {
			if (bi && v == p) continue;
			if (!detected && color[v] == 'G')
				color[v] = 'A', color[u] = 'R', detected = true;
			if (color[v] != 'B') continue;

			detected |= DetectarCiclo(v, u, detected);
			if (color[u] == 'G' && color[v] == 'R') color[u] = 'R';
		}
		if (color[u] == 'G') color[u] = 'N';
		return detected;
	}

	bool DetectarCiclo(int u) {
		color = vector<char>(n, 'B');
		return DetectarCiclo(u, u, false);
	}

	// Busqueda en amplitud.
	vector<int> BFS( int o ) {
	    vector<int> dist(n, INF);
	    queue<int> q; q.push(o); dist[o] = 0;
	    while (!q.empty()) {
	        int u = q.front(); q.pop();
	        for ( int v : aristas[u] ) {
	            if (dist[u] + 1 < dist[v]) {
	                dist[v] = dist[u] + 1;
	                q.push(v);
	            }
	        }
	    }
	    return dist;
	}

	// Detecta los puentes y puntos de articulacion en
	// un grafo bidireccional. Indices de 0 a n - 1.

	int tiempo;
	Lista low, label;

	vector<Lista> puentes;
	vector<bool> articulaciones;

	int PuentesArticulacion(int u, int p) {
		int hijos = 0;
		label[u] = low[u] = ++tiempo;
		for (int v : aristas[u]) {
			if (v == p) continue;
			if (label[v]) low[u] = min(low[u], label[v]);
			else {
				++hijos;
				PuentesArticulacion(v, u);
				if (label[u] > low[v]) {
					puentes[u].push_back(v);
					puentes[v].push_back(u);
				}
				if (label[u] >= low[v]) {
					articulaciones[u] = true;
				}
				low[u] = min(low[u], low[v]);
			}
		}
		return hijos;
	}

	int PuentesArticulacion() {
		tiempo = 0;
		low = Lista(n);
		label = Lista(n);

		puentes = vector<Lista>(n, Lista());
		articulaciones = vector<bool>(n);

		for (int u = 0; u < n; ++u) {
			if (label[u]) continue;
			int hijos = PuentesArticulacion(u, u);
			articulaciones[u] = hijos > 1;
		}
	}

	int top;
	vector<int> pila;
	vector<int> pertenece;
	vector<Lista> componentes;

	void FuertementeConexo(int u) {
		pila[top++] = u;
		label[u] = low[u] = ++tiempo;
		for (int v : aristas[u]) {
			if (!label[v]) FuertementeConexo(v);
			low[u] = min(low[u], low[v]);
		}
		if (label[u] == low[u]) {
			Lista componente;
			while (pila[top - 1] != u) {
				componente.push_back(pila[--top]);
				pertenece[pila[top]] = componentes.size();
				low[pila[top]] = INF;
			}
			componente.push_back(pila[--top]);
			pertenece[u] = componentes.size();
			low[u] = INF;
			componentes.push_back(componente);
		}
	}

};

typedef pair<int, int> Arista;

struct GrafoPonderado {

	int n;
	vector<vector<Arista>> aristas;

	GrafoPonderado(int N) : n(N),
		aristas(N, vector<Arista>()) {}

};

int main() {
	int n, m, u, v;
	scanf("%d%d", &n, &m);

	Grafo G(n);
	for (int i = 0; i < m; ++i) {
		scanf("%d%d", &u, &v);
		G.AgregarArista(u, v);
	}

	G.DetectarCiclo(0);
	for (int i = 0; i < n; ++i)
		printf("%c", G.color[i]);
	puts("");
	return 0;
}