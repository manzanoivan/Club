#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

const long long MOD = 1000000007LL;
// Factores primos de un numero a.
typedef long long Long;
typedef pair<Long, Long> Factor;

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
		if (n < primos[i]) break; Long p = (Long)primos[i];
		Long reps = n / p; while (primos[i] <= n / p)
			p *= primos[i], reps += n / p;
		factores.push_back(Factor(primos[i], reps));
	}
	return factores;
}

// Exponenciacion binaria a^n mod m.

Long Exponenciar(Long a, Long n, Long m) {
    Long resultado = 1ll;
    for (; n; n >>= 1ll) {
        if (n & 1ll) resultado =
            (resultado * a) % m;
        a = (a * a) % m;
    }
    return resultado;
}

Long Multiplicar(Long a, Long b, Long m) {
    Long resultado = 0ll;
    for (; b; b >>= 1ll) {
        if (b & 1ll) resultado =
            (resultado + a) % m;
        a = (a + a) % m;
    }
    return resultado;
}


// Algoritmo de Euclides extendido entre a y b.
// Además de devolver el gcd(a, b), resuelve la
// identidad de Bezout con el par (x, y). Si el
// parametro mod no es especificado, se resuelve
// con aritmetica normal; si mod se especifica,
// la identidad se resuelve modulo mod.

Long Euclides(Long a, Long b,
    Long& x, Long& y, Long mod = 0) {
    if (!b) { x = 1, y = 0; return a; }
    Long gcd = Euclides(b, a % b, x, y, mod);

    x = !mod? x - y * (a / b): (mod +
    	x - (y * (a / b)) % mod) % mod;
    swap(x, y); return gcd;
}

int main(){
	int casos, N;
	Long x, y;
	vector<int> criba = Criba( 5*10000 + 10 );
	vector<int> primos;
	for( int i = 2; i < criba.size(); i++ ){
		if( criba[i] == 0 ) primos.push_back( i );
	}
	cin >> casos;
	while( casos-- ){
		cin >> N;
		vector< Factor > fact = FactoresFactorial( N, primos );
		Long res = 1ll, temp;
		for( int i = 0; i < fact.size(); i++ ){
			temp = Exponenciar( fact[i].first, fact[i].second + 1ll, MOD) ;
			temp = (temp - 1ll + MOD) % MOD;
			Euclides( fact[i].first - 1, MOD, x, y, MOD );
			temp = (temp*x)% MOD ;
			res = (res * temp) % MOD;
		}

		Long restar = 1ll;
		for( Long i = 2ll; i <= N; i++ ){
			restar = ( restar * i ) % MOD;
		}

		//cout << restar << endl;
		res = ((res - restar) + MOD) % MOD;

		cout << res << '\n';
	}
	return 0;
}