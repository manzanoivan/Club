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
    //sort( P.begin(), P.end() );
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
double minimo = 1<<30;

void Bruta( int index, string cad ){
	if( index >= N ){
		double res = 0.0;
		int tam = cad.size();
		Poligono pols[9];
		for( int i = 0; i < tam; i++ )
			pols[ cad[i]-'0' ].push_back( puntos[ i ] );
		
		for( int i = 0; i < 9; i++ ){
			tam = pols[i].size();
			if( tam ){
				res += constante;
				pols[i] = CercoConvexo( pols[i] );
				res += Perimetro( pols[i] );
			}
		}
		minimo = min( minimo, res );
		return;
	}
	for( int i = 0; i < N; i++ )
		Bruta( index+1, cad+( (char)( i + '0' ) ) );
}

int main(){
	double x, y;
	int caso = 1;
	cout << fixed << setprecision(2);
	while( 1 ){
		cin >> N >> M;
		if( !N && !M ) break;

		puntos.clear();
		minimo = 1<<30;

		for( int i = 0; i < N; i++ ){
			cin >> x >> y;
			puntos.push_back( Punto(x, y) );
		}

		constante = M_PI * 2 * M;
		sort( puntos.begin(), puntos.end() );

		Bruta( 0, "" );

		cout << "Case "<< caso++ <<": length = " << minimo << '\n';
	}
	return 0;
}






