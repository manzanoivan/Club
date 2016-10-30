#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <sstream>
#include <iomanip>
using namespace std;

const double MAXN = 1e9;
const double ERROR = 1e-9;

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

	bool operator==( const Punto& cmp ) const{
		return Igual( x, cmp.x ) && Igual( y, cmp.y );
	}
};

double Cruz( const Punto& v, const Punto& w ){
	return v.x * w.y - v.y * w.x;
}

Punto Trasladar( const Punto& o, const Punto& p ){
	return Punto( p.x - o.x, p.y - o.y );
}

int ManoDerecha( const Punto& o, const Punto& p, const Punto& q ){
	double ccw = Cruz( Trasladar( o, p ), Trasladar( o, q ) );
	return Igual( ccw, 0 ) ? 0 : ( ccw < 0 ) ? -1 : 1;
}

struct Linea{
	Punto p, q;
	double a, b, c;

	Linea() : p(), q(), a(), b(), c(){}

	Linea( double A, double B, double C ) : p(), q(), a(A), b(B), c(C){
		if( Igual( a, 0 ) ){
			c /= -b; b = -1;
			p = Punto( 0, c );
			q = Punto( 1, c );
		}
		else if( Igual(b, 0) ){
			c /= -a; a = -1;
			p = Punto( c, 0 );
			q = Punto( c, 1 );
		}
		else{
			p = Punto( -c/a, 0 );
			q = Punto( -(b+c)/a, 1 );
		}
		if( q < p ) swap( p, q );
	}

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

	bool operator<( const Linea& cmp ) const{
		if( !Igual( a, cmp.a ) ) return a < cmp.a;
		if( !Igual( b, cmp.b ) ) return b < cmp.b;
		return Igual( c, cmp.c ) ? false : c < cmp.c;
	}
};

bool LineasParalelas( const Linea& l, const Linea& m ){
	if( Igual( l.b, 0 ) || Igual( m.b, 0 ) )
		return Igual( l.a, m.a ) && Igual( l.b, m.b );
	return Igual( l.a/l.b, m.a/m.b );
}

bool LineasIguales( const Linea& l, const Linea& m ){
	return LineasParalelas( l, m ) && Igual( l.c, m.c );
}

int IntersecRectaSegmen( const Linea& r, const Linea& s ){
	if( LineasIguales(r, s) ) return -1;
	if( LineasParalelas( r, s ) ) return 0;
	int t1 = ManoDerecha( r.p, r.q, s.p );
	int t2 = ManoDerecha( r.p, r.q, s.q );
	return ( t1 != t2 ) ? 1 : 0;
}

Punto PuntoInterseccion( const Linea& l, const Linea& m ){
	assert( !LineasParalelas(l, m) );
	if( Igual( l.a, 0 ) ) return Punto( (double)( l.c*m.b + m.c )/-m.a, l.c );
	double y = (double)( m.a*l.c - l.a*m.c ) / ( m.b*l.a - m.a*l.b );
	return Punto( (double)( l.c + l.b*y ) / -l.a, y );
}

typedef vector<Punto> Poligono;

bool RectaCortaPoligono( const Linea& r, const Poligono& P ){
	for( int i = 0, prim = 0; i < P.size(); i++ ){
		int lado = ManoDerecha( r.p, r.q, P[i] );
		if( !lado ) continue; if( !prim ) prim = lado;
		else if( lado != prim ) return true;
	}
	return false;
}

vector<Poligono> CortarPoligono( const Poligono& P, const Linea& r ){
	if( !RectaCortaPoligono( r, P ) ) return vector<Poligono>(1, P);
	int ind = 0, inter = 0 ;
	vector<Poligono> Ps(2);
	for( int i = 1; i < P.size(); i++ ){
		Linea s( P[i-1], P[i] );
		inter = IntersecRectaSegmen(r, s);
		if( inter ){
			Punto p = PuntoInterseccion( r, s );
			if( P[i-1] == p ) continue;
			Ps[ind].push_back( P[i-1] );
			Ps[1-ind].push_back( p );
			Ps[ind].push_back( p );
			ind = 1 - ind;
		}
		else
			Ps[ind].push_back( P[i-1] );
	}
	Ps[0].push_back( Ps[0][0] );
	Ps[1].push_back( Ps[1][0] );
	return Ps;
}

double Area( const Poligono& P ){
	double area = 0.0;
	for( int i = 1; i < P.size(); i++ )
		area += Cruz( P[i-1], P[i] );
	return fabs( area ) / 2.0;

}

int main(){
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	string X, Y;
	double temp;
	int E, D, N, C, x, y, index, caso = 1, res;
	char simbolo, operador;
	
	while( 1 ){
		cin >> E >> D >> N;
		if( !E && !D && !N ) break;

		Linea l;
		Poligono original;
		vector<Poligono> nuevos;

		original.push_back( Punto(0, 0) );
		original.push_back( Punto(E, 0) );
		original.push_back( Punto(E, D) );
		original.push_back( Punto(0, D) );
		original.push_back( Punto(0, 0) );
		res = 1;

		for( int i = 0; i < N; i++ ){
			cin >> X >> simbolo >> Y >> operador >> C;
			if( res == 0 ) continue;
			
			//stringstream ss( X.substr( 0, X.size()-3 ) );
			x = stoi( X.substr( 0, X.size()-3 ), nullptr );
			//stringstream ss2( Y.substr( 0, Y.size()-3 ) );
			//ss2 >> y;
			y = stoi( Y.substr( 0, Y.size()-3 ), nullptr );
			if( simbolo == '-' ) y = -y;
			
			index = 0;
			l = Linea( x, y, -C );
			nuevos = CortarPoligono( original, l );

			for( int j = 0; j < nuevos.size(); j++ ){
				for( int k = 0; k < nuevos[j].size(); k++ ){
					temp = x*nuevos[j][k].x + y*nuevos[j][k].y;
					if( !Igual( temp, C ) ){
						if( operador == '>' && temp > C){
							index = j+1;
							original = nuevos[j];
						}
						else if( operador == '<' && temp < C ){
							index = j+1;
							original = nuevos[j];
						}
						break;
					}
				}
			}
			
			if( index == 0 ){
				original.clear();
				original.push_back( Punto(0,0) );
				original.push_back( Punto(0,0) );
				original.push_back( Punto(0,0) );
				res = 0;
			}

		}
		cout << "Case #" <<  caso++ << ": ";
		cout << fixed << setprecision(2) << Area( original ) << '\n';

	}
	return 0;
}