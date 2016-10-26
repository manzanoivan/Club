#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;

const double ERROR = 1e-9;

// Tolerancia en flotantes.
bool Igual(double a, double b) { 
    return fabs(a - b) < ERROR;
}

// Punto en 2D.
struct Punto {
    double x, y;

    Punto() : x(), y() {}
    Punto(double X, double Y) : x(X), y(Y) {}
    // Izquierda a derecha, abajo a arriba.
    bool operator<(const Punto& cmp) const {
        if (!Igual(x, cmp.x)) return x < cmp.x;
        return Igual(y, cmp.y)? false: y < cmp.y;
    }
};


double Distancia(const Punto& p, const Punto& q) {
    return hypot(p.x - q.x, p.y - q.y);
}

double Cruz(const Punto& v, const Punto& w) {
    return v.x * w.y - v.y * w.x;
}

Punto Trasladar(const Punto& o, const Punto& p) {
    return Punto(p.x - o.x, p.y - o.y);
}

int ManoDerecha(const Punto& o, const Punto& p, const Punto& q) {
    double ccw = Cruz(Trasladar(o, p), Trasladar(o, q));
    return Igual(ccw, 0)? 0: (ccw < 0)? -1: 1;
}

typedef vector<Punto> Poligono;
// Cerco convexo de un conjunto de puntos.
Poligono CercoConvexo(vector<Punto> P){
    // Si ya esta ordenado, no usar sort.
    sort( P.begin(), P.end() );
    if( P.size() == 3 ){
        P.push_back( P[0] );
        return P;
    }
    Poligono arriba, abajo;
    for (int i = 0; i < P.size(); ++i) {
        while (arriba.size() > 1) {
            int p = arriba.size() - 1;
            // Permitir colineales: <=
            if (ManoDerecha(arriba[p - 1],
                arriba[p], P[i]) <= 0) break;
            arriba.pop_back();
        }
        arriba.push_back(P[i]);
    }
    arriba.pop_back();
    for (int i = P.size() - 1; i >= 0; --i) {
        while (abajo.size() > 1) {
            int p = abajo.size() - 1;
            //Permitir colineales: <=
            if (ManoDerecha(abajo[p - 1],
                abajo[p], P[i]) <= 0) break;
            abajo.pop_back();
        }
        abajo.push_back(P[i]);
    }
    arriba.insert(arriba.end(),
        abajo.begin(), abajo.end());
    return arriba; // Convex hull.
}

double Perimetro(const Poligono& P) {
    double perimetro = 0;
    for (int i = 1; i < P.size(); ++i)
        perimetro += Distancia(P[i - 1], P[i]);
    return perimetro;
}

int N;
double M;
Poligono puntos;
double constante;
double memo[600];

double dp( int estado ){
    if( memo[ estado ] != -1.0 ) return memo[ estado ];
    if( estado == 0 ) return memo[estado] = 0.0;

    vector< int > disponibles;
    for( int i = 0; i < 9; i++ )
        if( estado & ( 1 << i ) )
            disponibles.push_back( i );

    double minimo = 1 << 30;
    int tam = disponibles.size();
    int potencia = ( 1 << disponibles.size() );
    
    for( int i = 1; i < potencia; i++ ){
        Poligono temp;
        int copia = estado;
        for( int j = 0; j < tam; j++ ){
            if( i & ( 1 << j ) ){
                temp.push_back( puntos[ disponibles[j] ] );
                copia ^= (1 << disponibles[j]);
            }
        }

        minimo = min( minimo, constante + Perimetro( CercoConvexo( temp ) ) + dp( copia ) );
    }
    return memo[estado] = minimo;
}

void limpia(){
    for( int i = 0; i < 600; i++ )
        memo[i] = -1.0;
}

int main(){
    double x, y;
    int caso = 1;
    cout << fixed << setprecision(2);
    while( 1 ){
        cin >> N >> M;
        if( !N && !M ) break;

        puntos.clear();

        for( int i = 0; i < N; i++ ){
            cin >> x >> y;
            puntos.push_back( Punto(x, y) );
        }

        constante = M_PI * 2.0 * M;
        limpia();
        cout << "Case " << caso++ <<": length = " << dp( (1 << N) - 1 ) << '\n';
    }
    return 0;
}










