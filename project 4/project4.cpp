#define _USE_MATH_DEFINES
#include <xmmintrin.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <fstream>
#define SSE_WIDTH		4

#ifndef ARRAY_SIZE
#define ARRAY_SIZE 1000
#endif

#ifndef NUMTRIES
#define NUMTRIES   1000
#endif

float SimdMulSum( float *a, float *b, int len );
float nonSimdMulSum(float * a, float * b, int len);
float Ranf( float, float );


int main() {
	float* array1 = new float[ARRAY_SIZE];
	float* array2 = new float[ARRAY_SIZE];
	float performanceSimd = 0.;
	float perforanceNoSimd = 0.;
	double timewith;
	double timewithout;

	for(int i = 0; i < ARRAY_SIZE; i++) {
		array1[i] = Ranf(-1.f, 1.f);
		array2[i] = Ranf(-2.f,1.f);
	}

	float maxSimDPerformance = 0.0;
	float performance = 0.0;

	for(int i = 0; i < NUMTRIES; i++) {
		double time0 = omp_get_wtime();
		SimdMulSum(array1, array2, ARRAY_SIZE);
		double time1 = omp_get_wtime();
		timewith = time1-time0;

		double megaMultsPerSecond_Simd = (double)ARRAY_SIZE / (time1 - time0) / 1000000.;
		if(megaMultsPerSecond_Simd > maxSimDPerformance) {
			maxSimDPerformance = megaMultsPerSecond_Simd;
			// printf("timing: %f\n", time1 - time0);
		}

		double time2 = omp_get_wtime();
		nonSimdMulSum(array1, array2, ARRAY_SIZE);
		double time3 = omp_get_wtime();
		timewithout = time3-time2;

		double megaMults = (double)ARRAY_SIZE / (time3 - time2) / 1000000.;
		if(megaMults > performance) {
			performance = megaMults;
		}
	}
	
	std::ofstream outputfile;
	outputfile.open("output1.csv", std::ios_base::app);
	outputfile << ARRAY_SIZE << "," << performance << "," << maxSimDPerformance << "," << maxSimDPerformance/performance<< std::endl;
	printf("without simd: %f          with simd: %f\n", performance, maxSimDPerformance);
	printf("speedup: %lf       ARRAY SIZE: %d\n", maxSimDPerformance/performance, ARRAY_SIZE);

	delete[] array1;
	delete[] array2;

}






float
SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;

	__m128 ss = _mm_loadu_ps( &sum[0] );
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		ss = _mm_add_ps( ss, _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
	}
	_mm_storeu_ps( &sum[0], ss );

	for( int i = limit; i < len; i++ )
	{
		sum[0] += a[i] * b[i];
	}

	// printf("simd sum: %lf\n", sum[0] + sum[1] + sum[2] + sum[3]);
	return sum[0] + sum[1] + sum[2] + sum[3];
}

float nonSimdMulSum(float * a, float * b, int len) {
    float sum[4] = { 0., 0., 0., 0. };
    
    for( int i = 0; i < len; i++){
        sum[0] += a[i] * b[i];
    }
    
    // printf("Non-Simd: %lf\n", sum[0] + sum[1] + sum[2] + sum[3]);
    return sum[0] + sum[1] + sum[2] + sum[3];
}

float Ranf( float low, float high ) {
        float r = (float) rand();               // 0 - RAND_MAX
        float t = r  /  (float) RAND_MAX;       // 0. - 1.

        return   low  +  t * ( high - low );
}