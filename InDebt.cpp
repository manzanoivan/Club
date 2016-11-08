#include <iostream>
using namespace std;

int main(){
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	int acum, N, aux, res;
	while( 1 ){
		cin >> N;
		if( N < 0 ) break;
		acum = 0, res = 0;
		while( N-- ){
			cin >> aux;
			acum += aux;
			if( (acum % 100) == 0 ) res++;
		}
		cout << res << '\n';
	}

	return 0;
}