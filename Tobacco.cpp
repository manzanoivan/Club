#include <iostream>
#include <set>
using namespace std;

set< int > filas[4005], columnas[4005];

int main(){
	cin.tie(0);
	ios_base::sync_with_stdio(0);
	char tipo;
	string cad;
	int R, C, Q, x, y;

	cin >> R >> C >> Q;
	for( int i = 0; i < R; i++ ){
		cin >> cad;
		for( int j = 0; j < C; j++ ){
			if( cad[j] == 'X' ){
				filas[i].insert( j );
				columnas[j].insert( i );
			}
		}
	}

	set<int>::iterator it;

	while( Q-- ){
		cin >> x >> y >> tipo;
		--x, --y;
		if( tipo == 't' ){
			//cout << "update"<< *filas[ x ].find( y ) <<  endl;
			if( filas[ x ].find( y ) == filas[x].end() ){
				//cout << "Insertó" << endl;
				filas[x].insert( y );
				columnas[y].insert( x );
			}
			else{
				//cout << "Borró" << endl;
				filas[x].erase( filas[ x ].find( y ) );
				columnas[y].erase( columnas[y].find(x) );
			}
		}
		else{

			switch( tipo ){	
				case 'w':
					it = filas[x].lower_bound( y );
					if( *it == y && it != filas[x].end() ){
						cout << "0\n";
					}
					else if( it != filas[x].begin() ){
						cout << y - *(--it) << '\n';
					}
					else{
						cout << "No bad seeds found\n";
					}
				break;
				case 'e':
					it = filas[x].lower_bound( y );
					if( *it == y && it != filas[x].end() ){
						cout << "0\n";
					}
					else if( it != filas[x].end() ){
						cout << *(it)- y << '\n';
					}
					else{
						cout << "No bad seeds found\n";
					}
				break;
				case 'n':
					it = columnas[y].lower_bound( x );
					if( *it == x && it != columnas[y].end() ){
						cout << "0\n";
					}
					else if( it != columnas[y].begin() ){
						cout << x - *(--it) << '\n';
					}
					else{
						cout << "No bad seeds found\n";
					}
				break;
				case 's':
					it = columnas[y].lower_bound( x );
					if( *it == x && it != columnas[y].end() ){
						cout << "0\n";
					}
					else if( it != columnas[y].end() ){
						cout << *(it) - x << '\n';
					}
					else{
						cout << "No bad seeds found\n";
					}
				break;
			}
		}
	}

	return 0;
}