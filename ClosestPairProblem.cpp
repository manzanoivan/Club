#include <iostream>
#include <set>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

const double ERROR = 1e-6;

bool Igual( double a, double b ){
	return fabs( a - b ) < ERROR;
}

struct Punto{
	double x, y;
	Punto() : x(), y(){}
	Punto( double X, double Y ) : x(X), y(Y){}

	bool operator<( const Punto& cmp ) const{
		if( !Igual( x, cmp.x ) ) return x < cmp.x;
		return Igual( y, cmp.y ) ? false : y < cmp.y;
	}
};

double Distancia( const pair< Punto, Punto >& par ){
	Punto p = par.first, q = par.second;
	return hypot( p.x - q.x, p.y - q.y );
}

pair<Punto, Punto> ParMasCercano(vector<Punto> P) {
    // Si ya esta ordenado, no usar sort.
	sort(P.begin(), P.end());

	set<Punto> rect;
	pair<Punto, Punto> par;
	int prev = 0; double delta = 1e9;
	for (int i = 0; i < P.size(); ++i) {
		while (P[i].x - P[prev].x > delta)
			rect.erase(Punto(P[prev].y, P[prev++].x));

		set<Punto>::iterator it = rect.lower_bound(
			Punto(P[i].y - delta, P[0].x));

		for (; it != rect.end() && it->x <= P[i].y + delta; ++it) {
			double dist = hypot(P[i].x - it->y, P[i].y - it->x);
			if (dist < delta) delta = dist, par = make_pair(
				Punto(it->y, it->x), P[i]);
		}
		rect.insert(Punto(P[i].y, P[i].x));
	}
	return par; // Alternativamente puede devolver delta.
}

int main(){
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	int N;
	cout << fixed << setprecision(4);
	while( 1 ){
		cin >> N;
		if( !N ) break;

		vector<Punto> puntos(N);
		for( int i = 0; i < N; i++ ){
			cin >> puntos[i].x >> puntos[i].y;
		}

		double dist = Distancia( ParMasCercano( puntos ) );
		if( N > 1 && !Igual( dist, 10000.0 ) && dist < 10000.0 )
			cout << fixed << setprecision(4) << dist << '\n';
		else cout << "INFINITY\n";
	}
	return 0;
}
