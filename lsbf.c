#include "bloom.h"

int main(int nargs, char **args) {
	if(nargs < 5) {
		printf("Usage: %s #nDatasetPoints #nQueryPoints R datasetFilename querysetFilename\n", args[0]);
		return 0;
	}

	BLOOM **bloom;
	int i, d;
	int datasetNum, queryNum;
	double R;
	double *query;
	datasetNum = atoi(args[1]);
	queryNum = atoi(args[2]);
	R = atof(args[3]);
	getds(args[4], datasetNum);
	if(!(bloom = bloom_create(K_hfun, 5000)))
	{
		printf("ERROR: Could not create bloom filter\n");
		return -1;
	}
	
	bloom_set(bloom, dataset, datasetNum,R);
        

//query
	FILE *fp;
	fp = fopen(args[5], "r");
	query = (double*)malloc(dimention * sizeof(double));
	for(i = 0; i < queryNum; i++)
	{
		for(d = 0; d < dimention; d++)
		{
			fscanf(fp, "%lf", &query[d]);
		}
		printf("Query Point %d:", i);
		if(bloom_check_similar(bloom, query,R))
			printf("Yes\n");
		else
			printf("No\n");
	}
	bloom_destroy(bloom);
	free(query);
	fclose(fp);
	return 0;
}
