#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <tuple>
using namespace std;

typedef pair<int, int> Par;
typedef tuple<int, int, int, int> trio;

//trio 1: x, 2: tipo, 3: pos
// tipos: 1-punto, 2-inicio

struct FenTree {
    vector<int> tree;
    FenTree(int n) : tree(n + 1) {}
    
    void Actualizar(int i, int v) {
        while (i < tree.size()) {
            tree[i] += v;
            i += i & -i;
        }
    }
    
    int Query(int i) {
        int sum = 0;
        while (i > 0) {
            sum += tree[i];
            i -= i & -i;
        }
        return sum;
    }
    
    int Rango(int i, int j) {
        return Query(j) -
               Query(i - 1);
    }
};

int Mapeo( int num, vector<int>& mapa ){
	return lower_bound( mapa.begin(), mapa.end(), num ) - mapa.begin();
}

int main(){
	int P, V, x, y, query;
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	while( cin >> P >> V ){
		vector<Par> puntos;
		vector<trio> sweep;
		vector<int> mapeo;

		for( int i = 0; i < P; i++ ){
			cin >> x >> y;
			sweep.push_back(trio( x, 1, y, i+1 ));
			mapeo.push_back( y );
		}

		for( int i = 0; i < V; i++ ){
			cin >> x >> y;
			puntos.push_back( Par( x, y ) );
			mapeo.push_back( y );
		}

		sort( mapeo.begin(), mapeo.end() );
		vector<int>::iterator it = unique (mapeo.begin(), mapeo.end());
		mapeo.resize( distance(mapeo.begin(),it) );

		if( puntos[0].first == puntos[1].first ){
			for( int i = 0; i < V; i += 2 ){
				if( puntos[i].second > puntos[i+1].second ) swap( puntos[i], puntos[i+1] );
				sweep.push_back( trio( puntos[i].first, 2, puntos[i].second, puntos[i+1].second  ) );
			}
		}
		else{
			for( int i = 1; i < V-1; i += 2 ){
				if( puntos[i].second > puntos[i+1].second ) swap( puntos[i], puntos[i+1] );
				sweep.push_back( trio( puntos[i].first, 2, puntos[i].second, puntos[i+1].second ) );
			}
			if( puntos[0].second > puntos[V-1].second ) swap( puntos[0], puntos[V-1] );
			sweep.push_back( trio( puntos[0].first, 2, puntos[0].second, puntos[V-1].second ) );
		}

		FenTree fen( mapeo.size() + 5 );
		long long res = 0;
		sort( sweep.begin(), sweep.end() );
		
		for( int i = 0; i < sweep.size(); i++ ){
			//cout << "Evento " << sweep[i].first.first << " ";
			if( get<1>(sweep[i]) == 2 ){
				//cout << "Actualización "<< sweep[i].second.first <<"/" <<Mapeo( sweep[i].second.first, mapeo ) << " a " <<sweep[i].second.second<< "/"<<Mapeo( sweep[i].second.second, mapeo ) << endl;
				fen.Actualizar( Mapeo( get<2>(sweep[i]), mapeo )+1, 1 );
				fen.Actualizar( Mapeo( get<3>(sweep[i]), mapeo ) + 1, -1 );
			}
			else{
				//cout << "Query "<< fen.Query( Mapeo( sweep[i].second.first, mapeo )+1 ) <<endl;
				query = fen.Query( Mapeo( get<2>(sweep[i]), mapeo )+1 );
				if( !(query & 1) ){
					res += (long long)get<3>(sweep[i]);
				}
			}
		}
		cout << res << '\n';
	}
	return 0;
}