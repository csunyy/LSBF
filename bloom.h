#ifndef __BLOOM_H__
#define __BLOOM_H__

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "hash.h"
#define dimention 6	// dimension
#define BLOOM_L 4	// the number of bloom(parameter L in lsh)
#define K_hfun 3	// the number of h_function(parameter K in lsh)
#define W 4  		// The default value for lsh algorithm parameter W

#define CHAR_BIT 8
// 4294967291 = 2^32-5
#define UH_PRIME_DEFAULT 4294967291U
// 2^29
#define MAX_HASH_RND 536870912U     // lsh_r(1,MAX_HASH_RND)
unsigned *lsh_r; 	//used to calculate the gindex of lsh, gindex=((lsh_r*a)mod prime)mod tableSize
double **dataset; 	//data set
int dataNum; 		//the size of dataset

typedef struct
{
	int asize; 		// the size of a
	unsigned char *a;
	int nfuncs; 	// there are nfuncs function in a bloom(= K)
	double **para_a;// parameter a for lsh
	double *para_b; // parameter b for lsh

} BLOOM;



BLOOM **bloom_create(int nfuncs, int size);					// init L bloom filter
int bloom_destroy(BLOOM **bloom);							// destory L bloom filter
int bloom_set(BLOOM **bloom, double **dataset, int dataNum,double R);// set L bloom filter using dataset
int getds(char *s, int dsNum);					// read datasets

int getindex(BLOOM *bloom, unsigned *temp);					// get g(v) = ((lsh_r*h(v))mod prime)mod tableSize
unsigned int getvector(BLOOM* bloom, double *f,double R); 	// get temp[0...k-1] = [h1(v)...hk(v)]

int bloom_check_similar(BLOOM **bloom, double *s,double R);			// check whether point s is similar to dataset
void print_data(double *s);

#endif
