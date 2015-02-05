#include "bloom.h"
#include "hash.h"
#include <math.h>

#define SETBIT(a, n) (a[n/CHAR_BIT] |= (1<<(n%CHAR_BIT)))  // set a bit to 1
#define GETBIT(a, n) (a[n/CHAR_BIT] & (1<<(n%CHAR_BIT)))   // get a bit ,1 or 0

BLOOM **bloom_create(int nfuncs, int size)
{
	BLOOM **bloom;
	int i, l, k;
	// init g(v) functions
	if (!(lsh_r = (unsigned *) malloc(nfuncs * sizeof(unsigned))))
		return NULL;
	for (k = 0; k < nfuncs; k++)
	{
		lsh_r[k] = genRandomInt(1, MAX_HASH_RND);
	}

	// init L bloom filters
	if (!(bloom = (BLOOM **) malloc(BLOOM_L * sizeof(BLOOM *))))
		return NULL;
	for (i = 0; i < BLOOM_L; i++)
	{
		if (!(bloom[i] = (BLOOM *) malloc(sizeof(BLOOM))))
			return NULL;
		if (!(bloom[i]->a = calloc((size + CHAR_BIT - 1) / CHAR_BIT,
				sizeof(char))))
		{
			free(bloom[i]);
			free(bloom);
			return NULL;
		}
		bloom[i]->nfuncs = nfuncs;
		bloom[i]->asize = size;

		// init h(v) functions
		bloom[i]->para_a = (double **) malloc(nfuncs * sizeof(double *));
		for (k = 0; k < nfuncs; k++)
		{
			bloom[i]->para_a[k] = (double *) malloc(dimention * sizeof(double));
		}

		bloom[i]->para_b = (double *) malloc(nfuncs * sizeof(double));

		for (l = 0; l < bloom[i]->nfuncs; l++) 
		{
			for (k = 0; k < dimention; k++)
			{
				bloom[i]->para_a[l][k] = genGaussianRandom();
			}
			bloom[i]->para_b[l] = genUniformRandom(0, W);
		}
	}
	return bloom;
}

int bloom_destroy(BLOOM **bloom)
{
	int i, k;
	for (i = 0; i < BLOOM_L; i++)
	{
		free(bloom[i]->a);
		for (k = 0; k < bloom[i]->nfuncs; k++)
		{
			free(bloom[i]->para_a[k]);
		}
		free(bloom[i]->para_a);
		free(bloom[i]->para_b);
		free(bloom[i]);
	}
	free(bloom);
	for (i = 0; i < dataNum; i++)
		free(dataset[i]);
	free(dataset);
	return 0;
}

int getindex(BLOOM* bloom, unsigned *temp)
{
	int i;
	unsigned int index = 0;
	for (i = 0; i < bloom->nfuncs; i++)
	{
		index += temp[i] * lsh_r[i];
	}
	index %= UH_PRIME_DEFAULT;
	index %= bloom->asize;

	return index; 	//gIndex = g(v) =((lsh_r*h(v))mod prime)mod tableSize
        free(temp);
}

unsigned int getvector(BLOOM* bloom, double *f,double R) // in this function, we get h(v) and strore to temp
{
	int i, k;
        unsigned int *temp;
        temp = (unsigned *) malloc(sizeof(unsigned)*bloom->nfuncs);
	double result=0;
	for (i = 0; i < bloom->nfuncs; i++)
	{
		result = bloom->para_b[i];
		for (k = 0; k < dimention; k++)
		{
			result += f[k] * ((*(bloom->para_a[i] + k))/R);
		}
                result/=W;
		temp[i] = (unsigned) (floor(result)); // h(v) = (a.v+b)/w
	}
       return temp;
}

int bloom_set(BLOOM **bloom, double **dataset, int dataNum,double R) // add points(from dataset) to bloom
{
	int i, j, count = 0;
	unsigned int index, *temp = NULL;
	for (i = 0; i < BLOOM_L; i++)
	{
		count = 0;
		for (j = 0; j < dataNum; j++)
		{
			temp=getvector(bloom[i], dataset[j],R);
			index = getindex(bloom[i], temp);
			SETBIT(bloom[i]->a, index);
		}
	}
	
	return 0;
}

int bloom_check_similar(BLOOM **bloom, double *s,double R)
{
	// check whether a point is similar to a Set
	// for one bloom, if the point's	index +1 or -1 bit is 1,then we can say this bloom is true
	// if all bloom are true,then reutrn 1
	int i, j, t;
	unsigned int *temp, index;
	t = 0;
	for (i = 0; i < BLOOM_L; i++)
	{
		temp = getvector(bloom[i], s,R);
		index = getindex(bloom[i], temp);

		if (GETBIT(bloom[i]->a, index))
		{
			continue;
		}
		else
		{
			for (j = 0; j < bloom[i]->nfuncs; j++)
			{
				temp[j] -= 1;
				index = getindex(bloom[i], temp);
				if (GETBIT(bloom[i]->a, index))
				{
					break;
				}
				temp[j] += 2; 
				index = getindex(bloom[i], temp);
				if (GETBIT(bloom[i]->a, index))
				{
					break;
				}
				temp[j] -= 1;
			}
			if (j == bloom[i]->nfuncs)
			{
				free(temp);
				return 0;
			}
		}
	}
	
	free(temp);
	return 1;
}

int getds(char *s, int dsNum)
{
	FILE *fp;
	int i, d;
	fp = fopen(s, "r");
	dataset = (double**)malloc(dsNum * sizeof(double*));
	for(i = 0; i < dsNum; i++)
	{
		dataset[i] = (double*)malloc(dimention * sizeof(double));
		for(d = 0; d < dimention; d++)
		{
			fscanf(fp, "%lf", &dataset[i][d]);
		}
	}
	fclose(fp);
	return 1;
}

void print_data(double *s)
{
	int i;
	for (i = 0; i < dimention; i++)
	{
		printf("%lf ", s[i]);
	}
	printf("\n");
}
