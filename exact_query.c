#include "bloom.h"


// Returns 1 if the square of the Euclidean distance from point <p1> to <p2> is <=threshold.
int isDistancSqrLeq(double *p1, double *p2, double threshold)
{
	double result = 0;
	int i;
	for (i = 0; i < dimention; i++)
	{
		result += (p1[i] - p2[i]) * (p1[i] - p2[i]);
	}

	return result <= threshold;
}


double** getqs(char *s, int qsNum)
{
	FILE *fp;
	int i, d;
	fp = fopen(s, "r");
        double **queryset;
	queryset = (double**)malloc(qsNum * sizeof(double*));
	for(i = 0; i < qsNum; i++)
	{
		queryset[i] = (double*)malloc(dimention * sizeof(double));
		for(d = 0; d < dimention; d++)
		{
			fscanf(fp, "%lf", &queryset[i][d]);
		}
	}
	fclose(fp);
	return queryset;
}

int main(int nargs, char **args) {
	if(nargs < 5) {
		printf("Usage: %s #nDatasetPoints #nQueryPoints R datasetFilename querysetFilename\n", args[0]);
		return 0;
	}
        int i,j,temp,k=0;
        double **query;
        int datasetNum, queryNum;
        double R;
	datasetNum = atoi(args[1]);
	queryNum = atoi(args[2]);
        int result[queryNum];
	R = atof(args[3]);
        getds(args[4], datasetNum);
        query = getqs(args[5], queryNum);
        
  //query   
        for(i = 0; i < queryNum; i++)
	{ 
           result[i] = 0;
           for(j = 0;j < datasetNum;j ++)
           {
	      if(isDistancSqrLeq(dataset[j], query[i],R))
	          {
                         result[i] = 1;
                         continue;
                  }   
           }
          
         }
          for(k=0;k<queryNum;k++)
          {
               if(result[k] == 1) printf("Query Point %d:yes\n",k);
               else printf("Query Point %d:no\n",k);
           
	}
   free(query);
   return 0;

}
