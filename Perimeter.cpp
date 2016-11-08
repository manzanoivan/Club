#include <iostream>
using namespace std;

int N, x, y;
int mapa[109][109];
int visitado[109][109];
int movI[] = {0, 0, 1, -1};
int movJ[] = {1, -1, 0, 0};

bool valido( int i, int j ){
	if( i >= 0 && i <= 101 && j >= 0 && j <= 101) return true;
	return false;
}

int dfs1( int i, int j ){
	visitado[i][j] = 1;
	for( int k = 0; k < 4; k++ ){
		int u = movI[k] + i;
		int v = movJ[k] + j;
		if( valido( u, v ) && mapa[ u ][ v ] == 0 && !visitado[ u ][ v ]){
			dfs1( movI[k] + i , movJ[k] + j );
		}
	}
}

int dfs2( int i, int j ){
	int res = 0;
	visitado[i][j] = 1;
	for( int k = 0; k < 4; k++ ){
		int u = movI[k] + i;
		int v = movJ[k] + j;
		if( valido( u, v ) && mapa[u][v] == 0 && visitado[u][v] ){
			res++;
		}

		if( valido( u, v ) && mapa[ u ][ v ] == 1 && !visitado[ u ][ v ]){
			res += dfs2( movI[k] + i , movJ[k] + j );
		}
	}
	return res;
}

int main(){
	cin >> N;
	
	for( int i = 0; i < 109; i++ ){
		for( int j = 0; j < 109; j++ ){
			visitado[i][j] = 0;
		}
	}

	while( N-- ){
		cin >> x >> y;
		mapa[x][y] = 1;
	}

	dfs1( 0, 0 );

	cout << dfs2( x, y ) << '\n';

	return 0;
}