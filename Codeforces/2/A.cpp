#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

const int MAX = 1000000000;

vector< pair< string, int > > inst;
map< string, int > mapa;
set< string > ganadores;

int main(){
	int N;
	cin >> N;
	string ganador, nombre;
	int puntos, maximo = -MAX;

	while( N-- ){
		cin >> nombre >> puntos;
		inst.push_back( make_pair( nombre, puntos ) );
		mapa[ nombre ] += puntos;
	}

	for( auto it : mapa){
		if( it.second > maximo ) maximo = it.second;
	}

	for( auto it : mapa){
		if( it.second == maximo ) ganadores.insert( it.first );
	}

	mapa.clear();
	for( int i = 0; i < inst.size(); i++ ){
		if( ganadores.count( inst[i].first ) ){
			mapa[inst[i].first] += inst[i].second;
			if( mapa[inst[i].first] >= maximo ){
				ganador = inst[i].first;
				break;
			}
		}
	}

	cout << ganador << '\n';
	return 0;
}