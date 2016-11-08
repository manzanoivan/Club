#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long Long;

int N;
Long original;
Long num[25];
const int MAXM = 2000000;
int memo[ MAXM ];
Long tabla[25][25];
set< set<Long> > conjuntos;
int MAX;

Long GCD( Long a, Long b ){
	if( !b )
		return a;
	return GCD( b, a%b );
}

void dp( int mask ){
	if( mask == MAX ) return;
	if( memo[mask] != -1 ) return;

	vector< int > test;
	
	for( int i = 0; i < N; i++ )
		if( mask & ( 1 << i ) ) test.push_back( i );

	for( int i = 0; i < N; i++ ){
		if( !(mask & ( 1 << i ) ) ){
			bool band = true;

			for( int j = 0; j < test.size() && band; j++ ){
				if( tabla[ test[j] ][i] != 1LL )
					band = false;
			}

			if( band && tabla[N][i] == 1LL){
				set<Long> aux;
				for( int j = 0; j < test.size() ; j++ ){
					aux.insert( test[j] );
				}
				aux.insert( num[i] );
				conjuntos.insert( aux );
				dp(mask|( 1 << i ) );
			}
		}
	}
	memo[mask] = 1;
}

void limpia(){
	for( int j = 0; j < MAXM; j++ )
		memo[j] = -1;
	for( int i = 0; i < N; i++ ){
		for( int j = 0; j < N; j++ ){
			tabla[i][j] = GCD( num[i], num[j] );
		}
	}
	for( int j = 0; j < N; j++ ){
		tabla[N][j] = GCD( original, num[j] );
	}
}

int main(){
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	cin >> N >> original;
	for( int i = 0; i < N; i++ )
		cin >> num[i];

	MAX = (1 << N) - 1;

	limpia();
	dp( 0 );

	cout << conjuntos.size() << '\n';
	return 0;
}