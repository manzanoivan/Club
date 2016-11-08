#include <iostream>
using namespace std;

int main(){
	int N, aux;
	while( 1 ){
		cin >> N;
		if( !N ) break;

		while( N-- ){
			int res = -1;
			for( int i = 0; i < 5; i++ ){
				cin >> aux;
				if( aux <= 127 && res == -1){
					res = i;
				}
				else if( aux <= 127 && res != -1 ){
					res = -2;
				}
			}
			if( res < 0 ){
				cout << "*\n";
			}
			else{
				cout << (char)( 'A'+res )<<'\n';
			}
		}
	}
	return  0;
}