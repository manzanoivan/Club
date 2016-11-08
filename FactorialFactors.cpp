#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

const int MAXN = 100000;
typedef long long Long;
typedef pair<int, int> Factor;

// Criba de Eratostenes de 1 a n.

vector<int> Criba(int n) {
	int raiz = sqrt(n); vector<int> criba(n + 1);
	for (int i = 4; i <= n; i += 2) criba[i] = 2;
	for (int i = 3; i <= raiz; i += 2) if (!criba[i])
		for (int j = i * i; j <= n; j += i)
			if (!criba[j]) criba[j] = i;
	return criba;
}

// Factores primos de n factorial (n!).
// El vector de primos debe estar ordenado.

vector<Factor> FactoresFactorial(
	int n, const vector<int>& primos) {

	vector<Factor> factores;
	for (int i = 0; i < primos.size(); ++i) {
		if (n < primos[i]) break; int p = primos[i];
		int reps = n / p; while (primos[i] <= n / p)
			p *= primos[i], reps += n / p;
		factores.push_back(Factor(primos[i], reps));
	}
	return factores;
}

int main(){
	int casos, N;
	Long total, res;

	vector<int> criba = Criba( MAXN );
	vector<int> primos;
	for( int i = 2; i <= MAXN; i++ ){
		if( !criba[i] )primos.push_back( i );
	}

	cin >> casos;
	while( casos-- ){
		cin >> N;
		vector<Factor> fact = FactoresFactorial( N, primos );
		total = 0ll, res = 0ll;
		for( int i = 0; i < fact.size(); i++ ){
			if( fact[i].second > 0){
				total += fact[i].second;
				res++;
			}
		}
		cout << res << ' ' << total << '\n';
	}
	return 0;
}