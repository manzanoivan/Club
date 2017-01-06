#include <iostream>
using namespace std;
const int MAXN = 1009;
const int INF = 10000009;

int N;
int dos[MAXN][MAXN], cinco[MAXN][MAXN];
int dp[MAXN][MAXN], dq[MAXN][MAXN];

void limpia(){
	for( int i = 0; i < MAXN; i++ )
		for( int j = 0; j < MAXN; j++ ){
			dp[i][j] = -1;
			dq[i][j] = -1;
		}
}

int dpDos( int i, int j ){
	if( i == N || j == N ) return INF;
	if( i == N-1 && j == N-1 ) return dos[i][j];
	if( dp[i][j] != -1 ) return dp[i][j];
	
	return dp[i][j] = min( dpDos(i+1, j), dpDos( i, j+1 ) ) + dos[i][j];
}

int dpCinco( int i, int j ){
	if( i == N || j == N ) return INF;
	if( i == N-1 && j == N-1 ) return cinco[i][j];
	if( dq[i][j] != -1 ) return dq[i][j];
	
	return dq[i][j] = min( dpCinco(i+1, j), dpCinco( i, j+1 ) ) + cinco[i][j];
}

void reconstruyeDos( int i, int j ){
	if( i == N || j == N ) return;
	if( i == N-1 && j == N-1 ) return;
	if( dp[i][j] == dpDos(i+1, j) + dos[i][j] ){
		cout << 'D';
		reconstruyeDos( i+1, j );
	}
	else{
		cout << 'R';
		reconstruyeDos( i, j+1 );
	}
}

void reconstruyeCinco( int i, int j ){
	if( i == N || j == N ) return;
	if( i == N-1 && j == N-1 ) return;
	if( dq[i][j] == dpCinco(i+1, j) + cinco[i][j] ){
		cout << 'D';
		reconstruyeCinco( i+1, j );
	}
	else{
		cout << 'R';
		reconstruyeCinco( i, j+1 );
	}
}

int main(){
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	int num;
	int ii, jj;
	bool hayCero = false;

	cin >> N;

	limpia();

	for( int i = 0; i < N; i++ )
		for( int j = 0; j < N; j++ ){
			cin >> num;
			if( num == 0 ){
				ii = i;
				jj = j;
				hayCero = true;
				continue;
			}
			while( num%2 == 0 ){
				dos[i][j]++;
				num/= 2;
			}
			while( num%5 == 0 ){
				cinco[i][j]++;
				num/= 5;
			}
		}

	int res = min( dpDos(0, 0), dpCinco( 0, 0 ) );
	
	if( hayCero && res > 1 ){
		cout << "1\n";
		for( int i = 0; i < ii; i++ )
			cout << 'D';
		for( int j = 0; j < jj; j++ )
			cout << 'R';
		for( int i = 1; i < N-ii; i++ )
			cout << 'D';
		for( int j = 1; j < N-jj; j++ )
			cout << 'R';
	}
	else{
		cout << res << '\n';
		if( res == dpDos(0 , 0) )
			reconstruyeDos(0, 0);
		else
			reconstruyeCinco(0, 0);
	}
	cout << '\n';

	return 0;
}