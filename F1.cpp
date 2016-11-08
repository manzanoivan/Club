#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef pair<int, int> Par;
int puntos[109];
int resultados[109][109];

int main(){
	cin.tie();
	ios_base::sync_with_stdio(0);
	int G, P, Q, K;
	while( 1 ){
		cin >> G >> P;
		if( !G && !P ) break;

		for( int i = 0; i < 109; i++ ){
			for( int j = 0; j < 109; j++ ){
				resultados[i][j] = 0;
			}
		}

		for( int i = 0; i < G; i++ ){
			for( int j = 0; j < P; j++ ){
				cin >> resultados[i][j];
				--resultados[i][j];
			}
		}

		cin >> Q;

		while( Q-- ){
			cin >> K;

			for( int i = 0; i < 109; i++ )
				puntos[i] = 0;

			for( int i = 0; i < K; i++ ){
				cin >> puntos[i];
			}

			vector< Par > vect( P, Par( 0, 0 ) );
			for( int i = 0; i < P; i++ ){
				vect[i].second = -(i+1);
			}

			for( int i = 0; i < G; i++ ){
				for( int j = 0; j < P; j++ ){
					vect[j].first += puntos[ resultados[i][j] ];
				}
			}

			sort( vect.begin(), vect.end() );
			reverse( vect.begin(), vect.end() );

			int minimo = vect[0].first, cont = 0;

			for( int i = 0; i < P; i++ ){
				if( minimo == vect[i].first ){
					if( cont++ ) cout << ' ';
					cout << -vect[i].second;
				}
				else
					break;
			}
			cout << '\n';

		}

	}

	return 0;
}