#include <iostream>
using namespace std;

int N;
int valores[ 1000 ];

bool checaPrendidos(){
	for( int i = 1; i <= N; i++ ){
		if( valores[i] == i ) return true;
	}
	return false;
}

int main(){
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	bool band;
	while( 1 ){
		cin >> N;
		if( N < 0 ) break;

		band = true;
		for( int i = 1; i <= N; i++ ){
			cin >> valores[i];
			if( valores[i] > i ) band = false;
		}

		while( checaPrendidos() ){
			for( int i = 1; i <= N; i++ ){
				if( valores[i] == i ){
					valores[i] = 0;
					for( int j = i-1; j >= 1; j-- ){
						valores[j]++;
					}
					break;
				}
			}
		}

		for( int i = 1; i <= N; i++ ){
			if( valores[i] > 0 ) band = false;
		}

		if( band ) cout << "S\n";
		else cout << "N\n";

	}
	return 0;
}