#include <iostream>
#include <cmath>
#include <vector>
#include <map>
using namespace std;

typedef long long lli;

double ERROR = 1e-9;

bool Igual( double a, double b ){
	return fabs( a - b ) < ERROR;
}

struct Punto{
	double x, y;
	Punto() : x(), y(){}
	Punto( double X, double Y ) : x(X), y(Y){}
};

double Dot( const Punto& v, const Punto& w ){
	return v.x * w.x + v.y * w.y;
}

double Cruz( const Punto& v, const Punto& w ){
	return v.x * w.y - v.y * w.x;
}

Punto Trasladar( const Punto& o, const Punto& p ){
	return Punto( p.x - o.x, p.y - o.y );
}

double Magnitud( const Punto& v ){
	return hypot( v.x, v.y );
}

double Angulo( const Punto& v, const Punto& w ){
	return acos( Dot( v, w ) / (Magnitud(v) * Magnitud(w) ) );
}

int ManoDerecha( const Punto&o, const Punto& p, const Punto& q ){
	double ccw = Cruz( Trasladar( o, p ), Trasladar( o, q ) );
	return Igual( ccw, 0 ) ? 0 : ( ccw < 0 ) ? -1 : 1;
}

double RadAGrad( double rad ){
	return ( rad*180 )/M_PI;
}

struct Duble{
	double num;
	Duble( double N ) : num(N){}

	bool operator<(const Duble& cmp) const {
        if (Igual(num, cmp.num)) return false;
        return num < cmp.num;
    }
};

int main(){
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	int casos, N;
	lli gusto;
	double x, y, angulo;
	Punto punto;
	Punto normal( 1.0, 0.0 );
	Punto origen( 0.0, 0.0 );

	cin >> casos;
	while( casos-- ){
		cin >> N;

		map< Duble, lli > mapa;

		for( int i = 0; i < N; i++ ){
			cin >> gusto >> x >> y;
			punto = Punto( x, y );
			angulo = Angulo( punto, normal );
			if( ManoDerecha( origen, normal, punto ) == -1 ) angulo = 2*M_PI - angulo;
			mapa[ Duble(angulo) ] += gusto;
		}

		vector<lli> acum( 2*mapa.size() + 1, 0ll );
		int cont = 1;
		int tam = mapa.size();
		for( auto it : mapa ){
			acum[ cont+tam ] = it.second;
			acum[ cont++ ] = it.second;
		}

		map<lli, int> minimos;
		minimos[0ll] = 1;
		lli res = 0ll;

		for( int i = 1; i <= tam; i++ ){
			acum[i] += acum[i-1];
			res = max(acum[i] - minimos.begin()->first, res);
			minimos[ acum[i] ]++;
		}

		minimos[0ll]--;
		if( minimos[0ll] == 0 )
			minimos.erase( 0ll );

		for( int i = tam+1, j = 1; i <= 2*tam; i++, j++ ){
			acum[i] += acum[i-1];
			res = max(acum[i] - minimos.begin()->first, res);
			minimos[ acum[j] ]--;
			if( minimos[ acum[j] ] == 0 )
				minimos.erase( acum[j] );
			minimos[ acum[i] ]++;
		}
		
		cout << res << '\n';
	}

	return 0;
}