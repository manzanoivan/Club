#include <queue>
#include <iostream>
#include <vector>
#include <climits>
#include <cmath>
using namespace std;

// Definiciones iniciales.

typedef int Flujo; // Ajustable.

typedef vector<int> Lista;
typedef pair<int, int> Par;
typedef vector<Flujo> Flujo1D;
typedef vector<Flujo1D> Flujo2D;

const Flujo FINF = 1 << 30;

// Definiciones adicionales.

typedef int Costo; // Ajustable.

typedef vector<Costo> Costo1D;
typedef vector<Costo1D> Costo2D;
typedef pair<Costo, int> CostoNodo;
typedef pair<Flujo, Costo> FlujoCosto;

const Costo CINF = 1 << 30;

// FLUJO MEMORIA OPTIMIZADA Y
// FLUJO MAXIMO DE COSTO MINIMO
// Nodos indexados de 0 a n - 1.
// No utiliza matrices de adyacencia.

struct GrafoFlujoCosto {

    struct AristaFlujo {

        int dst; AristaFlujo* residual;
        Flujo cap, flujo; Costo peso, npeso;

        AristaFlujo(int d, Flujo f, Flujo c)
            : dst(d), flujo(f), cap(c) {}

        Costo AumentarFlujo(Flujo f) {
            residual->flujo -= f;
            this->flujo += f;
            return peso * f;
        }
    };

    int n; vector<Par> prv; Lista dist;
    vector< vector<AristaFlujo*> > aristas;

    GrafoFlujoCosto(int N) : n(N),
        aristas(N), prv(N), dist(N) {}
    
    ~GrafoFlujoCosto() { for (int i = 0; i < n; ++i)
        for (int j = 0; j < aristas[i].size(); ++j)
            delete aristas[i][j]; // NO OMITIR!!!
    }

    // Para aristas bidireccionales agreguen dos aristas
    // dirigidas. Si las aristas no son ponderadas dejen
    // el ultimo parametro con el valor por defecto.

    void AgregarArista(int u, int v, Flujo c, Costo p = 0) {
        AristaFlujo* uv = new AristaFlujo(v, 0, c);
        AristaFlujo* vu = new AristaFlujo(u, c, c);
        uv->residual = vu, vu->residual = uv;
        uv->peso = uv->npeso =  p;
        vu->peso = vu->npeso = -p;
        aristas[u].push_back(uv);
        aristas[v].push_back(vu);
    }

    // Flujo de costo minimo en O(VElogV * flow). Si dejan el
    // valor por defecto del parametro k saca el flujo maximo.

    void RecalcularCosto(const Costo1D& pi) {
        for (int u = 0; u < n; ++u) {
            for (int i = 0; i < aristas[u].size(); ++i) {
                AristaFlujo* v = aristas[u][i];
                v->npeso = v->npeso + pi[u] - pi[v->dst];
            }
        }
    }

    FlujoCosto ActualizarFlujo(int u, Flujo f) {
        int p = prv[u].first, i = prv[u].second;
        if (p == -1) return FlujoCosto(f, 0);
        AristaFlujo* pu = aristas[p][i];

        FlujoCosto res = ActualizarFlujo(
            p, min(f, pu->cap - pu->flujo));
        res.second += pu->AumentarFlujo(
            res.first); return res;
    }

    FlujoCosto AumentarFlujo(int s, int t, Flujo f) {
        Costo1D dist(n, CINF);
        fill(prv.begin(), prv.end(), Par(-1, -1));
        priority_queue<CostoNodo, vector<CostoNodo>,
                       greater<CostoNodo> > pq;
        pq.push(FlujoCosto(0, s)); dist[s] = 0;
        
        while (!pq.empty()) {
            int u = pq.top().second;
            Costo p = pq.top().first; pq.pop();
            if (dist[u] != p) continue;
            for (int i = 0; i < aristas[u].size(); ++i) {
                AristaFlujo* v = aristas[u][i];
                if (v->flujo == v->cap) continue;
                Costo ndist = dist[u] + v->npeso;
                if (ndist != dist[v->dst] &&
                           ndist < dist[v->dst]) {
                    dist[v->dst] = dist[u] + v->npeso;
                    pq.push(CostoNodo(ndist, v->dst));
                    prv[v->dst].second = i;
                    prv[v->dst].first = u;
                }
            }
        }
        if (dist[t] == CINF)
            return FlujoCosto(0, 0);
        RecalcularCosto(dist);
        return ActualizarFlujo(t, f);
    }

    FlujoCosto FlujoCostoMin(int s, int t, Flujo k = FINF) {
        Costo1D dist(n, CINF); dist[s] = 0;
        for (int i = 0; i < n; ++i) {
            for (int u = 0; u < n; ++u) {
                if (dist[u] == CINF) continue;
                for (int j = 0; j < aristas[u].size(); ++j) {
                    AristaFlujo* v = aristas[u][j];
                    if (v->flujo < v->cap) dist[v->dst] =
                        min(dist[v->dst], dist[u] + v->npeso);
                }
            }
        }
        RecalcularCosto(dist);

        FlujoCosto flujo_costo(0, 0);
        while (flujo_costo.first < k) {
            FlujoCosto fc = AumentarFlujo(
                s, t, k - flujo_costo.first);
            flujo_costo.second += fc.second;
            flujo_costo.first += fc.first;
            if (!fc.first) break;
        }
        return flujo_costo;
    }
};

typedef pair<int, Par> trio;

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    int casos, N, R, S, x, y, c, aux, u, v;

    cin >> casos;
    while( casos-- ){
        vector< trio > aristas;
        cin >> N >> R;
        for( int i = 0; i < R; i++ ){
            cin >> x >> y >> c;
            aristas.push_back( trio(c, Par( --x, --y )) );
        }

        vector<bool> duplicado( N, false ); 
        cin >> S;
        for( int i = 0; i < S; i++ ){
            cin >> aux;
            duplicado[--aux] = true;
        }

        GrafoFlujoCosto grafo( 2*N );

        for( int i = 0; i < aristas.size(); i++ ){
            u = 2*aristas[i].second.first + 1;
            v = 2*aristas[i].second.second;
            c = aristas[i].first;
            //cout << u << " " << v << " " << c << '\n';
            grafo.AgregarArista( u, v, 2, c );
            grafo.AgregarArista( v, u, 2, c );
        }

        for( int i = 0; i < N; i++ ){
            u = 2*i;
            v = 2*i + 1;
            //cout << u << " " << v << " " << ( duplicado[i] ? "UNO" : "FINF" ) << '\n';
            grafo.AgregarArista( u, v, ( duplicado[i] ? 1 : 2 ), 0 );
            grafo.AgregarArista( v, u, ( duplicado[i] ? 1 : 2 ), 0 );
        }

        FlujoCosto flujo = grafo.FlujoCostoMin( 0, 2*(N - 1) + 1, 2 );
        if( flujo.first == 2 )
            cout << flujo.second << '\n';
        else
            cout << "No safe route\n";

    }
    return 0;
}
