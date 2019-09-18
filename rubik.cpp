#include<iostream> 

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
	short mat[STICKERS];

	Cube()
	{
		for( int i = 0; i < STICKERS; ++ i )
			mat[i] = ( i / 8 );
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
			int tmp = mat[ ROTATIONS[idx][i] ];
			for( int j = 1; j < 4; ++ j )
				mat[ ROTATIONS[idx][ ( j - 1 ) * 3 + i ] ] = mat[ ROTATIONS[idx][ j * 3 + i ] ];
			mat[ ROTATIONS[idx][9 + i] ] = tmp;
		}
	}

	void print()
	{
		puts("");
		printf("      %d %d %d           \n", mat[8], mat[9], mat[10] );
		printf("      %d 1 %d           \n", mat[11], mat[12] );
		printf("      %d %d %d           \n", mat[13], mat[14], mat[15] );

		printf("%d %d %d %d %d %d %d %d %d %d %d %d\n", mat[0], mat[1], mat[2], mat[16], mat[17], mat[18], mat[32], mat[33], mat[34], mat[40], mat[41], mat[42] );
		printf("%d 0 %d %d 2 %d %d 4 %d %d 5 %d\n", mat[3], mat[4], mat[19], mat[20], mat[35], mat[36], mat[43], mat[44] ); 
		printf("%d %d %d %d %d %d %d %d %d %d %d %d\n", mat[5], mat[6], mat[7], mat[21], mat[22], mat[23], mat[37], mat[38], mat[39], mat[45], mat[46], mat[47] );
		
		printf("      %d %d %d           \n", mat[24], mat[25], mat[26] );
		printf("      %d 3 %d           \n", mat[27], mat[28] );
		printf("      %d %d %d           \n", mat[29], mat[30], mat[31] );
		puts("");
	}
	
	bool done()
	{
		for( int i = 0; i < STICKERS; ++ i )
			if( mat[i] != ( i / 8 ) ) 
				return false;
		return true;
	}
};

int cn = 0;
bool dfs( Cube * c, int depth = 0 )
{
	if( c->done() ) return true;
	//c->print();
	++cn;
	if( depth == 15 ) return false;
	for( int i = 0; i < TOTAL_ROTATIONS; ++ i )
	{
		c -> rotate( i );
		if( dfs( c, depth + 1 ) ) return true;
		c -> rotate( i ^ 1 );
	}
	return false;
}

int main()
{
	srand( time ( NULL ) );
	
	Cube * c = new Cube();
	c -> randomSort(15);
	c -> print();
	
	dfs( c );
	c->print();
	printf("%d\n", cn );
	return 0;
}
