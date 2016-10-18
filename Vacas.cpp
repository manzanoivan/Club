#include <iostream>
#include <climits>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

typedef int Flujo; // Ajustable.

typedef int Costo; // Ajustable.
typedef vector<int> Lista;
typedef pair<int, int> Par;
typedef vector<Flujo> Flujo1D;
typedef vector<Flujo1D> Flujo2D;

const Flujo FINF = 1 << 30;

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

    // Dinic para flujo maximo con memoria optimizada.
    // Prefieran esta version solo cuando n > 5,000.

    Flujo FlujoBloqueante(int u, int t, Flujo f) {
        if (u == t) return f; Flujo fluido = 0;
        for (int i = 0; i < aristas[u].size(); ++i) {
            if (fluido == f) break;
            AristaFlujo* v = aristas[u][i];
            if (dist[u] + 1 == dist[v->dst]) {
                Flujo fv = FlujoBloqueante(v->dst, t,
                    min(f - fluido, v->cap - v->flujo));
                v->AumentarFlujo(fv), fluido += fv;
            }
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
                    AristaFlujo* v = aristas[u][i];
                    if (dist[v->dst] < INT_MAX) continue;
                    if (v->flujo == v->cap) continue;
                    dist[v->dst] = dist[u] + 1;
                    q.push(v->dst);
                }
            }
            if (dist[t] < INT_MAX) flujo_maximo +=
                FlujoBloqueante(s, t, FINF);
        }
        return flujo_maximo;
    }
};

int N;
int mapa[105][105];
queue< Par > colas[2];

int movI[] = { 0, 0, 0, -1, 1 };
int movJ[] = { 0, -1, 1, 0, 0 };

bool valido( int a, int b ){
    if( a >= 0 && a < N && b >= 0 && b < N) return true;
    return false;
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int K, H, source, sink, num = 0, x, y;

    cin >> N >> K >> H;

    vector<Par> vacas( K );
    vector<int> niveles( H );
    GrafoFlujoCosto grafo( N*N*H*2 + N*N + 2 );
    vector<bool> marcados( N*N*H*2 + N*N + 2, false );

    for( int i = 0; i < N; i++ )
        for( int j = 0; j < N; j++ )
            cin >> mapa[i][j];

    source = N*N*H*2 + N*N;
    sink = source + 1;

    for( int i = 0; i < K; i++ )
        cin >> vacas[i].first >> vacas[i].second;

    for( int i = 0; i < H; i++ )
        cin >> niveles[i];

    for( int i = 0; i < K; i++ ){
        grafo.AgregarArista( source, vacas[i].first*N + vacas[i].second, 1 );
        colas[num].push( vacas[i] );
    }

    for( int i = 1; i < 2*H; i += 2){
        //cout << "Procesando i:" << i << endl;
        while( !colas[num].empty() ){
            x = colas[num].front().first;
            y = colas[num].front().second;
            //cout << "Procesando nodo: " << x << " " << y << endl;
            colas[num].pop();

            for( int k = 0; k < 5; k++ ){
                if( valido( x+movI[k], y+movJ[k] ) && mapa[ x+movI[k] ][ y+movJ[k] ] > niveles[ (( i + 1 ) / 2) - 1 ] ){
                    grafo.AgregarArista( x*N + y + ( i-1 )*N*N , (x+movI[k])*N + y+movJ[k] + i*N*N, 1 );
                    //cout << x*N + y + ( i-1 )*N*N << "->" << (x+movI[k])*N + y+movJ[k] + i*N*N << '\n';
                    if( !marcados[ (x+movI[k])*N + y+movJ[k] + (i+1)*N*N ] ){
                        //cout << "Van al: " << (x+movI[k])*N + y+movJ[k] + (i+1)*N*N << endl;
                        marcados[ (x+movI[k])*N + y+movJ[k] + (i+1)*N*N ] = true;
                        grafo.AgregarArista( (x+movI[k])*N + y+movJ[k] + i*N*N, (x+movI[k])*N + y+movJ[k] + (i+1)*N*N, 1 );
                        colas[ 1 - num ].push( Par(x+movI[k], y+movJ[k]) );
                    }
                }
            }
        }
        num = 1 - num;
    }

    while( !colas[num].empty() ){
        x = colas[num].front().first;
        y = colas[num].front().second;
        colas[num].pop();
        grafo.AgregarArista( x*N + y + 2*H*N*N , sink, 1 );
    }

    cout << grafo.Dinic( source, sink ) << '\n';

    return 0;
}