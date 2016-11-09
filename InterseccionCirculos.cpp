#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

// Definiciones iniciales.
typedef long long Long;

const double ERROR = 1e-9;
const double M_2PI = 2 * M_PI;

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
 
	bool operator==(const Punto& cmp) const {
		return Igual(x, cmp.x) && Igual(y, cmp.y);
	} 
};

// Distancia entre dos puntos p y q.
double Distancia(const Punto& p, const Punto& q) {
	return hypot(p.x - q.x, p.y - q.y);
}

// Magnitud de un vector v.
double Magnitud(const Punto& v) {
	return hypot(v.x, v.y);
}

// Producto punto entre vectores v y w.
double Dot(const Punto& v, const Punto& w) {
	return v.x * w.x + v.y * w.y;
}

// Producto cruz entre vectores v y w.
double Cruz(const Punto& v, const Punto& w) {
	return v.x * w.y - v.y * w.x;
}

// Conversion de grados a radianes.
double GradARad(double grd) {
	return (grd * M_PI) / 180;
}

// Conversion de radianes a grados.
double RadAGrad(double rad) {
	return (rad * 180) / M_PI;
}

// Rotar un punto respecto al origen.
// La rotación se hace en orden CCW, para
// rotar en CW llamar Rotar(p, M_2PI - rad).
Punto Rotar(const Punto& p, double rad) {
	return Punto(p.x*cos(rad) - p.y*sin(rad),
				 p.x*sin(rad) + p.y*cos(rad));
}

// Trasladar p tomando como origen al punto o .
Punto Trasladar(const Punto& o, const Punto& p) {
	return Punto(p.x - o.x, p.y - o.y);
}

// Escalar un vector v por un factor s.
Punto Escalar(const Punto& v, double s) {
	return Punto(v.x * s, v.y * s);
}

// Obtener vector opuesto.
Punto Opuesto(const Punto& v) {
	return Punto(-v.x, -v.y);
}

// Angulo entre vectores v y w.
double Angulo(const Punto& v, const Punto& w) {
	return acos(Dot(v, w) / (Magnitud(v) * Magnitud(w)));
}

// Test de mano derecha: CCW = 1, CW = -1, Colineal = 0.
int ManoDerecha(const Punto& o, const Punto& p, const Punto& q) {
	double ccw = Cruz(Trasladar(o, p), Trasladar(o, q));
	return Igual(ccw, 0)? 0: (ccw < 0)? -1: 1;
}

// Circulo en 2D.
struct Circulo {
	Punto c; double r; Circulo() : c(), r() {}
	Circulo(const Punto& C, double R) : c(C), r(R) {}

	bool operator<(const Circulo& cmp) const {
		if (!(c == cmp.c)) return c < cmp.c;
		return Igual(r, cmp.r)? false: r < cmp.r;
	}
};

// Circunferencia de un circulo.
double Circuferencia(const Circulo& c) {
	return M_2PI * c.r;
}

// Area de un circulo.
double Area(const Circulo& c) {
	return M_PI * c.r * c.r;
}

// Saber si un punto esta dentro de un circulo.
// En circunferencia = -1, Fuera = 0, Dentro = 1.
int PuntoEnCirculo(const Punto& p, const Circulo& c) {
	double dist = Distancia(p, c.c);
	if (Igual(dist, c.r)) return -1;
	return (dist < c.r)? 1: 0;
}

// Distancia de un punto p a un circulo c
double DistanciaPuntoCirculo(const Punto& p, const Circulo& c) {
	double dist = Distancia(p, c.c) - c.r;
	return (dist < 0)? 0: dist;
}
// Saber si un circulo c esta dentro de un circulo d.
bool CirculoEnCirculo(const Circulo& c, const Circulo& d) {
	return Distancia(c.c, d.c) < d.r - c.r;
}

// Saber si el circulo c intersecta con el circulo d.
// Uno dentro del otro = -1, Disjuntos = 0, Intersectan = 1.
int IntersecCirculoCirculo(const Circulo& c, const Circulo& d) {
	double dist = Distancia(c.c, d.c);
	if (dist < fabs(c.r - d.r)) return -1;
	return (dist > c.r + d.r)? 0: 1;
}

vector< Punto > PuntosInterseccionCirculos( const Circulo& c, const Circulo& d ){
	int mano;
	vector<Punto> ret;
	double angulo = 0.0, dist, X, Y;
	Circulo C = Circulo( Punto( 0, 0 ), c.r );
	Circulo D = Circulo( Trasladar( c.c, d.c ), d.r );
	mano = ManoDerecha( Punto(0, 0), Punto( 1, 0 ), D.c );
	
	if( mano == 1 )
		angulo = M_2PI - Angulo( Punto( 1, 0 ), D.c );
	else if( mano == -1 )
		angulo = Angulo( Punto( 1, 0 ), D.c );
	
	D.c = Rotar( D.c, angulo );
	dist = Distancia( D.c, C.c );
	
	if( Igual( dist, C.r + D.r ) ){
		ret.push_back( Punto( C.r, 0 ) );
		ret[0] = Rotar( ret[0], M_2PI - angulo );
		ret[0] = Trasladar( Punto( -c.c.x, -c.c.y ), ret[0] );
	}
	else if( dist < (C.r + D.r) && dist > fabs(C.r - D.r)) {
		X = (dist*dist - D.r*D.r + C.r*C.r) / (2*dist);
		Y = sqrt( C.r*C.r - X*X );
		ret.push_back( Punto( X, Y ) );
		ret.push_back( Punto( X, -Y ) );
		for( int i = 0; i < 2; i++ ){
			ret[i] = Rotar( ret[i], M_2PI - angulo );
			ret[i] = Trasladar( Punto( -c.c.x, -c.c.y ), ret[i] );
   		}
	}
	return ret;
}

int main(){
	double x, y, r;
	vector<Punto> puntos;
	vector<Circulo> circ;

	int N;
	cin >> N;
	for( int i = 0; i < N; i++ ){
		cin >> x >> y >> r;
		puntos.push_back( Punto( x, y ) );
		circ.push_back( Circulo( Punto( x, y ), r ) );
	}

	for( int i = 0; i < N; i++ ){
		for( int j = i+1; j < N; j++ ){
			vector<Punto> temp = PuntosInterseccionCirculos( circ[i], circ[j] );
			puntos.insert( puntos.end(), temp.begin(), temp.end() );
		}
	}

	int maximo = 0, cont;
	for( int i = 0; i < puntos.size(); i++){
		cont = 0;
		for( int j = 0; j < N; j++ ){
			if( PuntoEnCirculo( puntos[i], circ[j] ) != 0 ) cont++;
		}
		maximo = max( maximo, cont );
	}
	cout << maximo << '\n';

	return 0;
}