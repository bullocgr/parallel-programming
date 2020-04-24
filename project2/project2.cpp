#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <iostream>

// setting the number of threads:
#ifndef NUMT
#define NUMT		2
#endif

#ifndef NUMNODES
#define NUMNODES	10
#endif

#define N	      4
#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

float Height( int, int );
void TimeOfDaySeed( );

int main( int argc, char *argv[ ] ) {

    #ifndef _OPENMP
        fprintf( stderr, "No OpenMP support!\n" );
        return 1;
    #endif

    TimeOfDaySeed( );		// seed the random number generator
    omp_set_num_threads( NUMT );

    double time0 = omp_get_wtime( );
    #pragma omp parallel for default(none)
    for( int i = 0; i < NUMNODES*NUMNODES; i++ ) {
        int iu = i % NUMNODES;
        int iv = i / NUMNODES;
        float z = Height( iu, iv );
    }
	// the area of a single full-sized tile:

	float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:

	//need to check if it's a corner tile or a side tile
    //if it is corner is 1/4 the fulltilearea if side then it's 1/2

    double time1 = omp_get_wtime( );
    // double megaTrialsPerSecond = (double)NUMTRIALS / ( time1 - time0 ) / 1000000.;
}

// iu,iv = 0 .. NUMNODES-1
float Height( int iu, int iv )	 {
	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float r = 1. - xn - yn;
	if( r < 0. )
	        return 0.;
	float height = pow( 1. - xn - yn, 1./(float)N );
	return height;
}

void TimeOfDaySeed( ) {
	struct tm y2k = { 0 };
	y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
	y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	time_t  timer;
	time( &timer );
	double seconds = difftime( timer, mktime(&y2k) );
	unsigned int seed = (unsigned int)( 1000.*seconds );    // milliseconds
	srand( seed );
}