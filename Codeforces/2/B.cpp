#include <iostream>
using namespace std;
const int MAXN = 1009;
const int INF = 10000009;
#define Par pair< int, int >

int N;
Par mat[MAXN][MAXN];
Par memo[MAXN][MAXN];

void limpia(){
	for( int i = 0; i < MAXN; i++ )
		for( int j = 0; j < MAXN; j++ )
			memo[i][j] = Par( -1, -1 );
}

Par dp( int i, int j ){
	if( i == N || j == N ) return Par(INF,INF);
	if( i == N-1 && j == N-1 ) return mat[i][j];
	if( memo[i][j].first != -1 ) return memo[i][j];
	
	Par A = Par( dp( i+1, j ).first + mat[i][j].first, dp( i+1, j ).second + mat[i][j].second);
	Par B = Par( dp( i, j+1 ).first + mat[i][j].first, dp( i, j+1 ).second + mat[i][j].second );

	int minA = min( A.first, A.second );
	int minB = min( B.first, B.second );

	if( minA < minB ) return memo[i][j] = A;
	return memo[i][j] = B;
}

void reconstruye( int i, int j ){
	if( i == N || j == N ) return;
	if( i == N-1 && j == N-1 ) return;
	
	Par A = Par( dp( i+1, j ).first + mat[i][j].first, dp( i+1, j ).second + mat[i][j].second);
	Par B = Par( dp( i, j+1 ).first + mat[i][j].first, dp( i, j+1 ).second + mat[i][j].second );

	if( A == memo[i][j] ){
		cout << 'D';
		reconstruye( i+1, j );
	}
	else{
		cout << 'R';
		reconstruye( i, j+1 );
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
				mat[i][j].first++;
				num/= 2;
			}
			while( num%5 == 0 ){
				mat[i][j].second++;
				num/= 5;
			}
		}

	int res = min( dp( 0, 0 ).first, dp( 0, 0 ).second);
	
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
		reconstruye(0, 0);
	}
	cout << '\n';

	return 0;
}