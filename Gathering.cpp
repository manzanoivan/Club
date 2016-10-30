#include <iostream>
#include <set>
#include <algorithm>
using namespace std;

set< int > conjs[ 105 ];

int main(){
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	int vacio = 1, N, index = 1, temp;

	cin >> N;
	for( int i = 1; i <= N; i++ ){
		for( int j = 1; j <= N; j++ ){
			if( i == j ) continue;
			conjs[i].insert( j );
		}
	}

	cout << "1 1 0\n";

	while( !( conjs[index].size() == 1 && *conjs[ index ].begin() == 1) ){
		for( int it : conjs[index] ){
			if( conjs[it].size() > 0 ){
				temp = it;
				break;
			}
		}
		conjs[index].erase( conjs[index].find( temp ) );
		cout << temp << ' ' << temp << ' '<< index << '\n';
		index = temp;
	}

	for( int i = 1; i <= N; i++ ){
		if( conjs[i].size() > 0 ){
			cout << "0 1 " << i << '\n';
			break;
		}
	}

	return 0;
}