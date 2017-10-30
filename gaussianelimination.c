/*
	Part 2: Project 2
	Parallel & Distributed Programming
	Professor Striki
	
*/

//Command line arguments: ./openmp #1 #2  where #1 is the length of a square matrix and #2 is the number of threads desired.

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>

void finalOutputV(double* x, int N);
void Substitute(int N, double** A, double* b, double* x, int numThreads);
void pivotMatrix(int N, double** A, double* b, int currentPosition);
void matrixPrint(int N, double** A, double* b);
void *reduceMatrix(void *threadarg);
void checkAnswer(double** A, double* x, double* b, int N);
void generateMatrix(int N, double** A, double* b);

//initialize thread info to be used
struct ThreadInfo {
	double** A;
	double* b;
	int N;
	int j;
	int numThreads;
} threadInfo;

int main(int argc, char** argv){

	/* initialize matrices, vectors, and threads */
	int N = 3; //atoi(argv[1]); NOTE: This is commented out just to test this code
	int numThreads = 3; //atoi(argv[2]); NOTE: This is commented out just to test the code

	// define start time, elimination, and substituion
	struct timeval timeStart;
	struct timeval eliminationEnd;
	struct timeval substitutionEnd;

	//define elimination time, substitution time, and total time
	double timeElimination;
	double timeSubstitution;
	double timeTotal;
	pthread_t elimination_threads[numThreads];

	//create space for matrix A
	double **A = (double **)calloc(N,sizeof(double*));
	int q; // declare counter
	for (q=0; q < N; q++)
		A[q] = (double*)calloc(N,sizeof(double*));

	//Create space for vector b
	double* b = (double*) malloc(sizeof(double)*N);
	double* x = (double*) malloc(sizeof(double)*N);

	//Randomly generate numbers for matrix A
	generateMatrix(N, A, b);

	//condition on size of square matrix
	if (N <= 8)
	printf("\n Gaussian elimination with the matrix (A) and vector (b):\n\n");
	//print matrix
	matrixPrint(N, A, b);

	//declare info of threads
	threadInfo.A = A;
	threadInfo.b = b;
	threadInfo.N = N;
	threadInfo.numThreads = numThreads;

	//make an array for numThreads indices
	int *index = calloc (numThreads, sizeof (int));
	int i; // declare counter
	for(i = 0; i < numThreads; i++)
    {
    	index[i] = i;
    }

	gettimeofday(&timeStart, NULL);

	//Gaussian Elimination
	int j; // counter for loop
	for (j=0; j < N-1; j++){
		pivotMatrix(N, A, b, j);

		threadInfo.j = j;
        int thread_index; // loop counter
        int thread_index2;
		for (thread_index = 0; thread_index < numThreads; thread_index++){
			pthread_create(&elimination_threads[thread_index], NULL, reduceMatrix, (void*)&index[thread_index]);
		}

		for (thread_index2 = 0; thread_index2 < numThreads; thread_index2++){
			pthread_join(elimination_threads[thread_index], NULL);
		}
	}

	//to find the execution time for elimination
	gettimeofday(&eliminationEnd, NULL); 

	printf("\n-------Gaussian Elimination Complete-------\n");
	if (N <= 8){
		printf("\n Back substitution with the following matrix (A) and vector (b):\n\n");
		matrixPrint(N,A,b);
	}

	Substitute(N, A, b, x, numThreads);
	printf("\n  Back Substitution is Complete \n");

	//to find execution time for substitution 
	gettimeofday(&substitutionEnd, NULL); 

	//total times
	timeElimination = ((eliminationEnd.tv_sec  - timeStart.tv_sec) * 1000000u + eliminationEnd.tv_usec - timeStart.tv_usec) / 1.e6;
	timeSubstitution = ((substitutionEnd.tv_sec  - eliminationEnd.tv_sec) * 1000000u + substitutionEnd.tv_usec - eliminationEnd.tv_usec) / 1.e6;
	timeTotal = ((substitutionEnd.tv_sec  - timeStart.tv_sec) * 1000000u + substitutionEnd.tv_usec - timeStart.tv_usec) / 1.e6;

	finalOutputV(x, N);
	checkAnswer(A,x,b,N);

	printf("Elimination execution time: %.3f seconds.\n", timeElimination);
	printf("Substitution execution time: %.3f seconds.\n", timeSubstitution);
	printf("Total execution: \n%.3f seconds elapsed with %d threads used.\n\n", timeTotal, numThreads);
}



void pivotMatrix(int n, double** a, double* b, int j){

	int   i,k,m,rowx;
	double xfac, temp, temp1, amax;

	amax = (double) fabs(a[j][j]) ;
    m = j;
    for (i=j+1; i<n; i++){   /* Find the row with largest pivot */
    	xfac = (double) fabs(a[i][j]);
    	if(xfac > amax) {amax = xfac; m=i;}
    }

    if(m != j) {  /* Row interchanges */
    	rowx = rowx+1;
    	temp1 = b[j];
    	b[j]  = b[m];
    	b[m]  = temp1;
    	for(k=j; k<n; k++) {
    		temp = a[j][k];
    		a[j][k] = a[m][k];
    		a[m][k] = temp;
    	}
    }
}

void *reduceMatrix(void *threadarg){
	int *thread_index = (int*)threadarg;
	double** A = threadInfo.A;
	double* b = threadInfo.b;
	int N = threadInfo.N;
	int j = threadInfo.j;
	int numThreads = threadInfo.numThreads;
	int i, k;
	double m;

	for (k=j+1+ *thread_index; k<N; k += numThreads){
		m = A[k][j]/A[j][j];
		for (i=j; i<N; i++){
			A[k][i] = A[k][i] - (m * A[j][i]);
		}
		b[k] = b[k] - (m * b[j]);
	}
}

void generateMatrix(int N, double** A, double* b){

    /*int random[3][3];
    int i, o;
    srand(time(NULL));
    for(o = 0; o<3; o++)
        for(i = 0; i<3; i++)
            random[o][i] = rand();
    */
	srand(time(NULL));
	int i, j;
	for (i=0; i<N; i++){
		for (j=0; j<N; j++){
			A[i][j] = rand();
		}
		b[i] = rand();
	}
}


void finalOutputV(double* x, int N){
	if (N <= 8){
		printf("\nSolution Vector (x):\n\n");
		int i;
		for (i=0; i<N; i++){
			printf("|%10.6f|\n", x[i]);
		}
	}
}

void matrixPrint(int N, double** A, double* b){
	if (N <= 8){
		int x; // x counter for loop
		for (x=0; x<N; x++){
			printf("| ");
			int y; //counter for loop
			for(y=0; y<N; y++)
				printf("%7.2f ", A[x][y]);
			printf("| | %7.2f |\n", b[x]);
		}
	}
	else{
		printf("\nPerforming gaussian elimination with a matrix and vector too large to print out.\n");
		printf("If you would like to see output, try again with a matrix of length 8 or less.\n");
	}
}

void checkAnswer(double** A, double* x, double* b, int N){

	double* result_vector = (double*) malloc(sizeof(double)*N);
	double row_sum;
    int j;
	for (j=0; j<N; j++){
		row_sum = 0;
		int k; // program counter
		for (k=0; k<N; k++){
			row_sum += A[j][k]*x[k];
		}
		result_vector[j] = row_sum;
	}

	double sumOfSquares = 0;
	double entryOfResidual;
	int i;
	for (i=0; i<N; i++){
		entryOfResidual = result_vector[i] - b[i];
		sumOfSquares += entryOfResidual*entryOfResidual;
	}
	sumOfSquares = sqrt(sumOfSquares);
	printf("\nThe L2-Norm of the result vector from Ax-b is: %.20f\n", sumOfSquares);
}


void Substitute(int N, double** A, double* b, double* x, int numThreads){
	int i,j;
	for (i=N-1; i >= 0; i--){
		x[i] = b[i];
		for (j=i+1; j<N; j++){
			x[i] -= A[i][j]*x[j];
		}
		x[i] = x[i] / A[i][i];
	}
}
