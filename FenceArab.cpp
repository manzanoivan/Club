#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

const double ERROR = 1e-9;
const double M_2PI = 2 * M_PI;

bool Igual( double a, double b ){
	return fabs( a - b ) < ERROR;
}

struct Punto{
	double x, y;
	Punto() : x(), y(){}
	Punto( double X, double Y ) : x(X), y(Y){}

	bool operator<( const Punto& cmp ) const {
		if( !Igual( x, cmp.x ) ) return x < cmp.x;
		return Igual( y, cmp.y ) ? false : y < cmp.y;
	}

	bool operator==( const Punto& cmp ) const {
		return Igual( x, cmp.x ) && Igual( y, cmp.y );
	}
};

double Magnitud( const Punto& v ){
	return hypot( v.x, v.y );
}

double Distancia( const Punto& p, const Punto& q ){
	return hypot( p.x - q.x, p.y - q.y );
}

double Dot( const Punto& v, const Punto& w ){
	return v.x*w.x + v.y*w.y;
}

double Cruz( const Punto& v, const Punto& w ){
	return v.x*w.y - v.y*w.x;
}

double Angulo( const Punto& v, const Punto& w ){
	return acos( Dot( v, w ) / (Magnitud(v) * Magnitud(w) ) );
}

Punto Opuesto( const Punto& v ){
	return Punto( -v.x, -v.y );
}

Punto Trasladar( const Punto& o, const Punto& p ){
	return Punto( p.x - o.x, p.y - o.y );
}

Punto Escalar( const Punto& v, double s ){
	return Punto( v.x * s, v.y * s );
}

Punto Rotar( const Punto& p, double rad ){
	return Punto( p.x*cos(rad) - p.y*sin(rad) ,
				  p.x*sin(rad) + p.y*cos(rad) );
}

int ManoDerecha( const Punto& o, const Punto& p, const Punto& q ){
	double ccw = Cruz( Trasladar( o, p ), Trasladar( o, q ) );
	return Igual( ccw, 0 ) ? 0 : ( ccw < 0 ) ? -1 : 1;
}

struct Linea{
	Punto p, q;
	double a, b, c;

	Linea() : p(), q(), a(), b(), c(){}

	Linea( const Punto& P, const Punto& Q ) : p(P), q(Q), a(), b(), c() {
		if( q < p ) swap( p, q );
		a = q.y - p.y;
		b = p.x - q.x;
		if( !a ) c = p.y, b = -1;
		else if( !b ) c = p.x, a = -1;
		else{
			c = -a*p.x - b*p.y;
		}
	}
};

Linea PerpendicularEnPunto( const Linea& l, const Punto& p ){
	return Linea( p, Punto( p.x + l.a, p.y + l.b ) );
}

struct Circulo{
	Punto c; double r;
	Circulo() : c(), r(){}

	Circulo( const Punto& C, double R ) : c(C), r(R){}
};

double DistanciaPuntoCirculo( const Punto& p, const Circulo& c ){
	double dist = Distancia( p, c.c ) - c.r;
	return ( dist < 0 ) ? 0 : dist;
}

Punto ProyPuntoCircunferencia( const Punto& p, const Circulo& c ){
	Punto v = Trasladar( p, c.c );
	double prop = DistanciaPuntoCirculo( p, c ) / Magnitud(v);
	return Trasladar( Opuesto(p), Escalar( v, prop ) );
}

Linea ProyTangentes( const Punto& p, const Circulo c ){
	double a = acos( c.r / Distancia( p, c.c ) );
	Punto p_ = Trasladar( c.c, ProyPuntoCircunferencia(p, c) );
	return Linea( Trasladar( Opuesto(c.c), Rotar( p_, M_2PI - a ) ),
				  Trasladar( Opuesto(c.c), Rotar( p_, a) ) );
}

bool CirculoEnCirculo( const Circulo& c, const Circulo& d ){
	return Distancia( c.c, d.c ) < d.r - c.r;
}

int PuntoEnCirculo( const Punto& p, const Circulo& c ){
	double dist = Distancia( p, c.c );
	if( Igual( dist, c.r ) ) return -1;
	return ( dist < c.r ) ? 1 : 0;
}

int TangenteExtCirculo( const Circulo& a, const Circulo& b, Linea& s, Linea& t ){
	if( Igual( a.r, b.r ) && a.c == b.c ) return 0;

	Punto u;
	bool unico = false;
	if( b.r < a.r && Igual( Distancia( a.c, b.c ) + b.r, a.r ) ){
		u = Trasladar( Opuesto( a.c ), Escalar( Trasladar( a.c, b.c ), a.r / Distancia( a.c, b.c ) ) );
		unico = true;
	}
	if( a.r < b.r && Igual( Distancia( a.c, b.c ) + a.r, b.r ) ){
		u = Trasladar( Opuesto( b.c ), Escalar( Trasladar( b.c, a.c ), b.r / Distancia( a.c, b.c ) ) );
		unico = true;
	}
	if( unico ){
		s = t = PerpendicularEnPunto( Linea( a.c, b.c ), u );
		return 1;
	}
	if( CirculoEnCirculo( a, b ) || CirculoEnCirculo( b, a ) ) return 0;

	Punto v;
	Linea proy;

	if( Igual( a.r, b.r ) ){
		proy = Linea( a.c, a.c );
		Linea perp = PerpendicularEnPunto( Linea( a.c, b.c ), a.c );
		u = Escalar( Trasladar( perp.q, perp.p ), b.r / Distancia( perp.p, perp.q ) );
		v = Opuesto( u );
	}
	else{
		Circulo c( a.c, abs( a.r - b.r ) );
		proy = ProyTangentes( b.c, c );
		u = Escalar( Trasladar( a.c, proy.p ), b.r / ( a.r - b.r ) );
		v = Escalar( Trasladar( a.c, proy.q ), b.r / ( a.r - b.r ) );
	}

	s = Linea( Trasladar( Opuesto( proy.p ), u ), Trasladar( Opuesto( b.c ), u ) );
	t = Linea( Trasladar( Opuesto( proy.q ), v ), Trasladar( Opuesto( b.c ), v ) );
	return 2;
}

typedef pair<Punto, int> Par;
typedef vector<Par> Poligono;

Poligono CercoConvexo( vector<Par> P ){
	sort( P.begin(), P.end() );
	Poligono arriba, abajo;
	for( int i = 0; i < P.size(); i++ ){
		while( arriba.size() > 1 ){
			int p = arriba.size() - 1;
			if( ManoDerecha( arriba[p-1].first, arriba[p].first, P[i].first ) < 0 ) break;
			arriba.pop_back();
		}
		arriba.push_back( P[i] );
	}
	arriba.pop_back();
	for( int i = P.size() - 1; i >= 0; --i ){
		while( abajo.size() > 1 ){
			int p = abajo.size() - 1;
			if( ManoDerecha( abajo[p-1].first, abajo[p].first, P[i].first ) < 0 ) break;
			abajo.pop_back();
		}
		abajo.push_back( P[i] );
	}
	arriba.insert( arriba.end(), abajo.begin(), abajo.end() );
	return arriba;
}

int main(){
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	double X, Y, R;
	int casos, N, ini;
	cin >> casos;

	cout << fixed << setprecision(5);
	while( casos-- ){
		cin >> N;
		vector<Circulo> circulos;
		for( int i = 0; i < N; i++ ){
			cin >> X >> Y >> R;
			circulos.push_back( Circulo( Punto(X, Y), R ) );
		}

		if( N == 1 ){
			cout << M_2PI * R << '\n';
			continue;
		}

		Poligono pol;

		for( int i = 0; i < N; i++ ){
			for( int j = i+1; j < N; j++ ){
				Linea l, m;
				TangenteExtCirculo( circulos[i], circulos[j], l, m );
				
				if( PuntoEnCirculo( l.p, circulos[i] ) ){
					pol.push_back( Par( l.p, i ) );
					pol.push_back( Par( l.q, j ) );
				}
				else{
					pol.push_back( Par( l.q, i ) );
					pol.push_back( Par( l.p, j ) );
				}

				if( PuntoEnCirculo( m.p, circulos[i] ) ){
					pol.push_back( Par( m.p, i ) );
					pol.push_back( Par( m.q, j ) );
				}
				else{
					pol.push_back( Par( m.q, i ) );
					pol.push_back( Par( m.p, j ) );
				}
			}
		}

		pol = CercoConvexo( pol );
		double res = 0.0;
		int rightHand = ManoDerecha( pol[0].first, pol[1].first, pol[2].first ), temp;
		//cout << rightHand << endl;
		for( int i = 1; i < pol.size(); i++ ){
			if( pol[i].second == pol[ i-1 ].second ){
				temp = ManoDerecha( pol[i-1].first, pol[i].first, circulos[ pol[i].second ].c );
				//cout << temp << endl;
				if( temp != rightHand ){
					res += circulos[ pol[i].second ].r * ( M_2PI - Angulo( Trasladar( circulos[ pol[i].second ].c, pol[i-1].first ), Trasladar( circulos[ pol[i].second ].c, pol[i].first ) ) );
				}
				else{
					res += circulos[ pol[i].second ].r * ( Angulo( Trasladar( circulos[ pol[i].second ].c, pol[i-1].first ), Trasladar( circulos[ pol[i].second ].c, pol[i].first ) ) );
				}
			}
			else{
				res += Distancia( pol[i].first, pol[i-1].first );
			}
		}
		cout << res << '\n';

	}

	return 0;
}