#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Globals
unsigned int seed = 0;
int    	NowYear;		// 2020 - 2025
int	    NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	    NowNumDeer;		// number of deer in the current population
bool    fire;


const float GRAIN_GROWS_PER_MONTH =		20.0;
const float ONE_DEER_EATS_PER_MONTH =	1.0;

const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			    2.0;	// plus or minus noise

const float AVG_TEMP =				    60.0;	// average
const float AMP_TEMP =				    20.0;	// plus or minus
const float RANDOM_TEMP =			    10.0;	// plus or minus noise

const float MIDTEMP =				    40.0;
const float MIDPRECIP =			    	10.0;

const int ENDYEAR =                     2026;

// Function Prototypes
float SQR(float x);
float Ranf( unsigned int *seedp, float low, float high );
int Ranf( unsigned int *seedp, int ilow, int ihigh );
float getAng();
float getTemp();
float getPrecip();
void GrainDeer();
void Grain();
void MyAgent();
void Watcher();

int main( int argc, char *argv[] ) {
	omp_set_num_threads( 4 );

	// starting date and time:
	NowMonth =    0;
	NowYear  = 2020;

	// starting state (feel free to change this if you want):
	NowNumDeer = 1;
	NowHeight =  1.;

    while(NowYear <= ENDYEAR) {    
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                GrainDeer();
            }

            #pragma omp section
            {
                Grain();
            }

            #pragma omp section
            {
                MyAgent();
            }

            #pragma omp section
            {
                Watcher();
            }
        }
    }

    return 0;
}

float SQR( float x ) {
        return x*x;
}

float Ranf( unsigned int *seedp, float low, float high ) {
        float r = (float) rand_r( seedp ); // 0 - RAND_MAX

        return( low + r * ( high - low ) / (float)RAND_MAX );
}

int Ranf( unsigned int *seedp, int ilow, int ihigh ) {
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)( Ranf(seedp, low,high) );
}

float getAng() {
    return (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
}

float getTemp() {
    float temp = AVG_TEMP - AMP_TEMP * cos( getAng() );
    // unsigned int seed = 0;
    return temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );
}

float getPrecip() {
    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( getAng() );
    precip += Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    if ( precip < 0. ) {
    	precip = 0.;
    }

    return precip;
}

void GrainDeer() {
    int spawnDeer, deadDeer;
        deadDeer = 0;
        spawnDeer = 0;
        if(NowNumDeer > NowHeight) {
            deadDeer++;
        } else {
            spawnDeer++;
        }
    

        #pragma omp barrier

        NowNumDeer += spawnDeer;
        NowNumDeer -= deadDeer;

        #pragma omp barrier

        #pragma omp barrier
}

void Grain() {
	int deadGrain;
		float tempFactor   = exp( -SQR( ( NowTemp - MIDTEMP ) / 10. ) );
		float precipFactor = exp( -SQR( ( NowPrecip - MIDPRECIP ) / 10. ) );

		if(fire) {
            deadGrain = NowHeight - Ranf(&seed, 0., NowHeight-1);
        }

		#pragma omp barrier

		NowHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
	    NowHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

		if (NowHeight < 0) {
			NowHeight = .5;
		} else if (NowHeight > 100) {
			NowHeight = 50;
		}

		#pragma omp barrier

		#pragma omp barrier
}

void MyAgent() {
        if(NowHeight > 10) {
            fire = true;
        } else {
            fire = false;
        }

		#pragma omp barrier

		#pragma omp barrier

		#pragma omp barrier
}

void Watcher() {
    int updateYear, updateMonth, updateTemp, updatePrecip;
	
        updateMonth = NowMonth + 1;
		if (updateMonth > 11) {
			updateYear = NowYear + 1;
			updateMonth = 0;
		} else {
			updateYear = NowYear;
		}

		updateTemp = getTemp();
		updatePrecip = getPrecip();

		#pragma omp barrier

		NowYear = updateYear;
		NowMonth = updateMonth;
		NowTemp = updateTemp;
		NowPrecip = updatePrecip;

		#pragma omp barrier
		printf("MONTH: %d         YEAR: %d        TEMP: %f          PRECIP: %f          DEER: %d           GRAIN: %f            FIRE: %d\n", NowMonth + 1, NowYear, (5./9.)*(NowTemp-32.), NowPrecip*2.54, NowNumDeer, NowHeight*2.54, fire);

		#pragma omp barrier
}