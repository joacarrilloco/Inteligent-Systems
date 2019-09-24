#include<iostream> 
#include<map>
#include<queue>

using namespace std;

const int STICKERS = 48;
const int ROT_SIZE = 12;
const int TOTAL_ROTATIONS = 12;
const int ROTATIONS[TOTAL_ROTATIONS][ROT_SIZE] = 
{
	{0,1,2,16,17,18,32,33,34,40,41,42},
	{42,41,40,34,33,32,18,17,16,2,1,0},
	{5,6,7,21,22,23,37,38,39,45,46,47},
	{47,46,45,39,38,37,23,22,21,7,6,5},
	{8,11,13,16,19,21,24,27,29,47,44,42},
	{42,44,47,29,27,24,21,19,16,13,11,8},
	{10,12,15,18,20,23,26,28,31,45,43,40},
	{40,43,45,31,28,26,23,20,18,15,12,10},
	{29,30,31,39,36,34,10,9,8,0,3,5},
	{5,3,0,8,9,10,34,36,39,31,30,29},
	{24,25,26,37,35,32,15,14,13,2,4,7},
	{7,4,2,13,14,15,32,35,37,26,25,24}
};

struct Cube
{
	string mat;

	Cube()
	{
		mat = "";
		for( int i = 0; i < STICKERS; ++ i )
			mat.push_back('0' + ( i / 8 ));
	}
	
	void randomSort( int movements )
	{
		if( movements == 0 ) return;
		this -> rotate( rand() % 12 );	
		this -> randomSort( movements - 1 );
	}

	void rotate( int idx )
	{
		for( int i = 0; i < 3; ++ i )
		{
			char tmp = mat[ ROTATIONS[idx][i] ];
			for( int j = 1; j < 4; ++ j )
				mat[ ROTATIONS[idx][ ( j - 1 ) * 3 + i ] ] = mat[ ROTATIONS[idx][ j * 3 + i ] ];
			mat[ ROTATIONS[idx][9 + i] ] = tmp;
		}
	}

	void print()
	{
		puts("");
		printf("      %c %c %c           \n", mat[8], mat[9], mat[10] );
		printf("      %c 1 %c           \n", mat[11], mat[12] );
		printf("      %c %c %c           \n", mat[13], mat[14], mat[15] );

		printf("%c %c %c %c %c %c %c %c %c %c %c %c\n", mat[0], mat[1], mat[2], mat[16], mat[17], mat[18], mat[32], mat[33], mat[34], mat[40], mat[41], mat[42] );
		printf("%c 0 %c %c 2 %c %c 4 %c %c 5 %c\n", mat[3], mat[4], mat[19], mat[20], mat[35], mat[36], mat[43], mat[44] ); 
		printf("%c %c %c %c %c %c %c %c %c %c %c %c\n", mat[5], mat[6], mat[7], mat[21], mat[22], mat[23], mat[37], mat[38], mat[39], mat[45], mat[46], mat[47] );
		
		printf("      %c %c %c           \n", mat[24], mat[25], mat[26] );
		printf("      %c 3 %c           \n", mat[27], mat[28] );
		printf("      %c %c %c           \n", mat[29], mat[30], mat[31] );
		puts("");
	}
	
	bool done()
	{
		for( int i = 0; i < STICKERS; ++ i )
			if( mat[i] != '0' + ( i / 8 ) ) 
				return false;
		return true;
	}
};

namespace DFS
{
	map< string, short > M0;
	void init( Cube * c, int depth = 0, int lastMovement = -1 )
	{
		if( M0.count( c->mat ) && M0[ c->mat ] <= depth ) return;
		M0[ c->mat ] = depth;

		if( depth == 6 ) return;

		for( int i = 0; i < TOTAL_ROTATIONS; ++ i )
		{
			if( i == lastMovement ) continue;
			c -> rotate( i );
			init( c, depth + 1, i ^ 1 );
			c -> rotate( i ^ 1 );
		}
	}

	short search( Cube * c, int depth = 0, int lastMovement = -1 )
	{
		if( M0.count( c -> mat ) ) 
			return depth + M0[ c -> mat] ;
		if( depth == 9 ) return false;
		for( int i = 0; i < TOTAL_ROTATIONS; ++ i )
		{
			if( i == lastMovement ) continue;
			c -> rotate( i );
			short r = search( c, depth + 1, i ^ 1 );
			if( r ) return r; 
				
			c -> rotate( i ^ 1 );
		}
		return false;
	}

	void recolectStats()
	{
		puts("\n\n**************************************\nDFS\n\n");
		init(new Cube());
		for( int it = 1; it <= 30; ++ it )
		{
			Cube * c =  new Cube();
			c -> randomSort(15);
			search( c );
			printf(" %d", it );
			fflush(stdout);
		}
		puts("\n");
		printf("Memory (Kb) userd for precalculation: %d\n", int( M0.size() ) * 48 / 1024 );

		printf("");
		printf("");

		puts("**************************************\n");
	}
};


namespace BFS
{
	map< string, short > M0;
	void init( Cube * c )
	{
		queue < string > q;
		q.push( c -> mat );

		int dep = 0;
		while( q.size() && dep < 7 )
		{
			int sz = q.size();
			for( int it = 0; it < sz; ++ it )
			{
				string s = q.front();
				q.pop();
				c -> mat = s;
				for( int i = 0; i < TOTAL_ROTATIONS; ++ i )
				{
					c -> rotate( i );
					if( M0.count( c -> mat ) == 0 )
					{
						q.push( c -> mat );
						M0[c-> mat] = dep + 1;
					}
					c -> rotate( i ^ 1 );
				}
			}
			++dep;
		}
	}

	short search( Cube * c )
	{
		queue < string > q;
		q.push( c -> mat );
		
		int dep = 0;
		while( q.size() )
		{
			int s = q.size();
			for( int it = 0; it < s; ++ it )
			{
				string s = q.front();
				q.pop();

				if( M0.count( s ) ) return dep + M0[s];
				c -> mat = s;
				for( int i = 0; i < TOTAL_ROTATIONS; ++ i )
				{
					c -> rotate( i );
					q.push( c -> mat );
					c -> rotate( i ^ 1 );
				}
			}
			++dep;
		}
		return -1;
	}

	void recolectStats()
	{
		puts("\n\n**************************************\nBFS\n\n");
		init(new Cube());
		for( int it = 1; it <= 30; ++ it )
		{
			Cube * c =  new Cube();
			c -> randomSort(15);
			search( c );
			printf(" %d", it );
			fflush(stdout);
		}
		puts("");

		printf("Memory (Kb) userd for precalculation: %d\n", int( M0.size() ) * 48 / 1024 );
		printf("");
		printf("");

		puts("**************************************\n");
	}
};

int main()
{
	srand( time ( NULL ) );
	DFS::recolectStats();
	BFS::recolectStats();
	return 0;
}
