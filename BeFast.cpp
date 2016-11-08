#include <iostream>
#include <vector>
using namespace std;

// Definiciones iniciales.

typedef int Flujo; // Ajustable.

typedef vector<int> Lista;
typedef pair<int, int> Par;
typedef vector<Flujo> Flujo1D;
typedef vector<Flujo1D> Flujo2D;

const Flujo FINF = 1 << 30;

// EMPAREJAMIENTO BIPARTITO
// Nodos indexados de 0 a n - 1.

struct Bipartito {

    int n; Lista pareja;
    vector<Lista> aristas;
    vector<bool> lado, visitado;

    Bipartito(int N) : lado(N), pareja(N),
        visitado(N), aristas(N), n(N) {}

    void AgregarArista(int u, int v) {
        aristas[u].push_back(v);
        aristas[v].push_back(u);
    }

    void AgregarIzq(int u) { lado[u] = true; }
    void AgregarDer(int u) { lado[u] = false; }

    int CaminoIncremental(int u) {
        visitado[u] = true;
        for (int i = 0; i < aristas[u].size(); ++i)
            if (pareja[aristas[u][i]] == -1) 
                return pareja[aristas[u][i]] = u;
        for (int i = 0; i < aristas[u].size(); ++i) {
            int v = aristas[u][i];
            if (visitado[pareja[v]]) continue;
            if (CaminoIncremental(pareja[v]) != -1)
                return pareja[v] = u;
        }
        return -1;
    }

    vector<Par> MaxEmparejamiento() {
        fill(pareja.begin(), pareja.end(), -1);
        for (int i = 0; i < n; ++i) {
            if (!lado[i]) continue; CaminoIncremental(i);
            fill(visitado.begin(), visitado.end(), false);
        }
        vector<Par> pares;
        for (int i = 0; i < n; ++i)
            if (!lado[i] && pareja[i] != -1)
                pares.push_back(Par(pareja[i], i));
        return pares; // Cardinalidad = pares.size()
    }
};

int main(){
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	int N, M, x, y;
	while( 1 ){
		cin >> N >> M;
		if( !N && !M ) break;

		Bipartito bip( N + N + 2 );
		for( int i = 0; i < N; i++ ){
			bip.AgregarIzq(i);
			bip.AgregarDer(i+N);
		}

		for( int i = 0; i < M; i++ ){
			cin >> x >> y;
			bip.AgregarArista( x, y + N );
		}
		//cout << "llega" << endl;
		cout << (bip.MaxEmparejamiento()).size() << '\n';
	}

	return 0;
}