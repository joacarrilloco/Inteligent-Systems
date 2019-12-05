#include<bits/stdc++.h>
using namespace std;

const int STICKERS = 48;
const int ROT_SIZE = 12;
const int TOTAL_ROTATIONS = 12;
const int FACE_SIZE = 8;

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

const int FACE_ROTATION[TOTAL_ROTATIONS][FACE_SIZE] = 
{
  {11,14,12,9,13,15,10,8},
	{8,10,15,13,9,12,14,11},
  {26,31,29,24,28,30,27,25},
  {25,27,30,28,24,29,31,26},
  {6,3,1,4,5,0,2,7},
  {7,2,0,5,4,1,3,6},
  {35,38,36,33,37,39,34,32},
  {32,34,39,37,33,36,38,35},
  {44,46,43,41,42,47,45,40},
  {40,45,47,42,41,43,46,44},
  {19,22,20,17,21,23,18,16},
  {16,18,23,21,17,20,22,19},
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

    for( int i = 0; i < 2; ++ i )
    {
      char tmp = mat[ FACE_ROTATION[idx][4 * i] ];
      for( int j = 0; j < 3; ++ j )
        mat[ FACE_ROTATION[idx][4* i + j] ] = mat[FACE_ROTATION[idx][4*i + j + 1]];
     mat[ FACE_ROTATION[idx][4*i + 3] ] = tmp;
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

  const short inv[6] = {4,3,5,1,0,2};
  short heuristic()
  {
    return 0;
    int tot = 0;
    for( int i = 0; i < STICKERS; ++ i )
    {
      short v = ( mat[i] - '0' );
      tot += ( v != i / 8 );
      tot += ( v == inv[i/8] );
    }
    return ( tot + 11 ) / 12;
  }
        
};

namespace DFS
{
	map< string, short > M0;
	long long sum_STsize;
	int cn_st;
	void init( Cube * c, int depth = 0, int lastMovement = -1 )
	{
		++cn_st;
		sum_STsize += depth;

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
		++cn_st;
		sum_STsize += depth;

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
		cn_st = 0;
		sum_STsize = 0;
		puts("\n\n**************************************\nDFS\n\n");
		double in_time = clock();
		init(new Cube());
		int sum = 0;
		for( int it = 1; it <= 30; ++ it )
		{
			Cube * c =  new Cube();
			c -> randomSort(15);
			sum += search( c );
			printf(" %d", it );
			fflush(stdout);
		}
		puts("\n");
		printf("Memory (Kb) userd for precalculation: %d\n", int( M0.size() ) * 48 / 1024 );
		printf("Average movements used for solving: %0.2f\n", 1.0 * sum / 30 );
		printf("Average time used for solving: %0.2f secs\n", (clock() - in_time) / CLOCKS_PER_SEC / 30.0 );
		printf("Maximum stack size %d\n", 9 );
		printf("Average stack size %0.2f\n", 1.0 * sum_STsize / cn_st );
		printf("\n");

		puts("**************************************\n");
	}
};


namespace BFS
{
	map< string, short > M0;
	int mx_Qsize, cnQ;
	long long sum_Qsize;
	void init( Cube * c )
	{
		queue < string > q;
		M0[ c-> mat ] = 0;
		q.push( c -> mat );

		int dep = 0;
		while( q.size() && dep < 7 )
		{
			int sz = q.size();
			for( int it = 0; it < sz; ++ it )
			{
				++cnQ;
				sum_Qsize += int( q.size() );
				if( int( q.size() ) > mx_Qsize )
					mx_Qsize = int( q.size() );
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
    if( M0.count( c -> mat ) )
      return M0[c -> mat ];

		queue < string > q;
		q.push( c -> mat );

		int dep = 0;
		while( q.size() )
		{
			int s = q.size();
			for( int it = 0; it < s; ++ it )
			{
				if( int( q.size() ) > mx_Qsize )
					mx_Qsize = int( q.size() );
				string s = q.front();
				q.pop();

				c -> mat = s;
				for( int i = 0; i < TOTAL_ROTATIONS; ++ i )
				{
					c -> rotate( i );
					if( M0.count( c -> mat ) )
						return dep + M0[ c-> mat ] + 1;
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
		mx_Qsize = cnQ = 0;
		sum_Qsize = 0;
		puts("\n\n**************************************\nBFS\n\n");
		double in_time = clock();
		init(new Cube());
		int sum = 0;
		for( int it = 1; it <= 30; ++ it )
		{
			Cube * c =  new Cube();
			c -> randomSort(15);
			sum +=search( c );
			printf(" %d", it );
			fflush(stdout);
		}
		puts("");

		printf("Memory (Kb) userd for precalculation: %d\n", int( M0.size() ) * 48 / 1024 );
		printf("Average movements used for solving: %0.2f\n", 1.0 * sum / 30 );
		printf("Average time used for solving: %0.2f secs\n", (clock() - in_time) / CLOCKS_PER_SEC / 30.0 );
		printf("Maximum queue size %d\n", mx_Qsize );
		printf("Average queue size %0.2f\n", 1.0 * sum_Qsize / cnQ );

		printf("\n");

		puts("**************************************\n");
	}
};

namespace Aasterisk
{
 	map< string, short > M0;
	int mx_Qsize, cnQ;
	long long sum_Qsize;
	void init( Cube * c )
	{
		queue < string > q;
		M0[ c-> mat ] = 0;
		q.push( c -> mat );

		int dep = 0;
		while( q.size() && dep < 7 )
		{
			int sz = q.size();
			for( int it = 0; it < sz; ++ it )
			{
				++cnQ;
				sum_Qsize += int( q.size() );
				if( int( q.size() ) > mx_Qsize )
					mx_Qsize = int( q.size() );
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

  int search( Cube c )
  {
    if( M0.count( c.mat ) )
      return M0[ c.mat ];

    queue < pair<int,Cube> > q [22];
    q[c.heuristic()].push({0,c});

    int it = 0, currQSize = 0;
    while( true )
    {
      if( q[it].size() == 0 )
      {
        ++it;
        continue;
      }

      sum_Qsize += currQSize;
      mx_Qsize = max( mx_Qsize, currQSize );
      Cube curr = q[it].front().second;
      int depth = q[it].front().first;
      
      q[it].pop();

      --currQSize;
      ++depth;

      for( int i = 0; i < TOTAL_ROTATIONS; ++ i )
      { 
        curr.rotate( i );
        
        if( M0.count( curr.mat ))
          return depth + M0[curr.mat];
        else
          q[depth + curr.heuristic()].push({ depth, curr } ), ++currQSize;

        curr.rotate( i ^ 1 );
      }
    }
    return -1;
  }

	void recolectStats()
	{
		mx_Qsize = cnQ = 0;
		sum_Qsize = 0;
		puts("\n\n**************************************\nA*\n\n");
		double in_time = clock();
    init(new Cube());
    int sum = 0;
		for( int it = 1; it <= 30; ++ it )
		{
			Cube c;
			c.randomSort(15);
			sum+=search( c );
			printf(" %d", it );
			fflush(stdout);
		}
		puts("");

		printf("Memory (Kb) userd for precalculation: %d\n", int( M0.size() ) * 48 / 1024 );
		printf("Average movements used for solving: %0.2f\n", 1.0 * sum / 30 );
		printf("Average time used for solving: %0.2f secs\n", (clock() - in_time) / CLOCKS_PER_SEC / 30.0 );
		printf("Maximum queue size %d\n", mx_Qsize );
		printf("Average queue size %0.2f\n", 1.0 * sum_Qsize / cnQ );

		printf("\n");

		puts("**************************************\n");
	}
};

int main()
{
	srand( time ( NULL ) );
//	DFS::recolectStats();
	BFS::recolectStats();
//	Aasterisk ::recolectStats();

  /*
  https://rubiks-cube-solver.com/solution.php?cube=0336646114625511663231435221334462565322151235154425446
  for( int i = 0; i < 12; ++ i )
  {
    Cube c;
    c.rotate( i );

    cout << i << endl;
    c.print();
  }

  Cube c;
  c.mat = "435504422244400132015330051151142211354523300325";

  cout <<"GO!" << endl;
  c.print();
  c.rotate(11);
  c.print();
  c.rotate(6);
  c.print();
  c.rotate(3);
  c.print();
  c.rotate(8);
  c.print();
  c.rotate(0);
  c.print();
  c.rotate(6);
  c.print();
  c.rotate(11);
  c.print();
  c.rotate(0);
  c.print();
  c.rotate(0);
  c.print();
  c.rotate(11);
  c.print();
  c.rotate(3);
  c.print();
  c.rotate(4);
  c.print();
  c.rotate(9);
  c.print();
  c.rotate(9);
  c.print();
  c.rotate(6);
  c.print();
  c.rotate(6);
  c.print();
  c.rotate(1);
  c.print();
  c.rotate(10);
  c.print();
  c.rotate(10);
  c.print();
  c.rotate(5);
  c.print();
  c.rotate(5);
  c.print();
  c.rotate(3);
  c.print();
  c.rotate(9);
  c.print();
  c.rotate(9);
  c.print();
  c.rotate(5);
  c.print();
  c.rotate(5);
  c.print();
  
  */
	return 0;
}

