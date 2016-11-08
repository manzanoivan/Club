#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

int main(){
	int casos;
	double A, B, C, P;

	cin >> casos;
	while(casos--){
		cin >> A >> B >> C;
		P = (A+B+C)/2;
		cout << fixed << setprecision(1) << 7*sqrt( P * ( P - A ) * (P - B) * ( P - C ) ) << '\n';
	}
}