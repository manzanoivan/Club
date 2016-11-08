#include <iostream>
#include <vector>
using namespace std;

typedef long long Long;
const Long INF = 1ll << 60ll;

Long ceros, unos, temp, aux;

Long calcula( int conjuntos ){
	Long res = 0ll;
	//cout << "Conjuntos: " << conjuntos << endl;
	temp = ceros - conjuntos + 1 ;
	res += temp*temp;
	temp = conjuntos-1;
	res += temp;
	//cout << res << endl;
	
	temp = unos/(conjuntos+1);
	aux = unos % ( conjuntos + 1 );
	res -= (temp*temp) * ( conjuntos+1-aux );
	temp++;
	res -= (temp*temp) * (aux );
	//cout << res << '\n';
	return res;
}

int main(){
	int casos;
	cin >> casos;

	while( casos-- ){
		cin >> ceros >> unos;
		Long res = -INF;
		for( int i = 1; i <= ceros && i <= unos; i++ ){
			res = max( res, calcula( i ) );
		}
		if( ceros == 0 )
			res = -unos*unos;
		if( unos == 0 )
			res = ceros*ceros;
		cout << res << '\n';
	}

	return 0;
}