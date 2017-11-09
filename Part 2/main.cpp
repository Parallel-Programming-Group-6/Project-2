// Stephan Dimitrovski, Bony Patel, Jayan Ariyawansa, Michael Lacsamana
// Parallel & Distributed Programming
// Project 2 Part 2

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <pthread.h>
#include <vector>
#include <iomanip>
#include "timing.h"
#include "gaussian.h"
using namespace std;

static const int MAX_THREADS = 8;

//Data types for index and number of threads
struct passVals {
	vector<vector<double> > *A;
	vector<double> *b;
	int N;
	int i;
	int numThreads;
} passVals;

void *eliminate(void *args){
	int *thread_index = (int*)args;
	vector<vector<double> > &A = *passVals.A;
	vector<double> &b = *passVals.b;
	int N = passVals.N;
	int j = passVals.i;
	int numThreads = passVals.numThreads;
	double m;

	for (int k = j + 1 + *thread_index; k < N; k += numThreads){
		m = A[k][j]/A[j][j];
		for (int i = j; i < N; i++){
			A[k][i] = A[k][i] - (m * A[j][i]);
		}
		b[k] = b[k] - (m * b[j]);
	}
}

int main(){
	
	//for generating a random matrix
	srand(time(NULL));
	int N = rand() % 10 + 2;
	
	//Declare for timing
	double startTime;
	double endTime;
	double serialTime;
	pthread_t tid[MAX_THREADS];

	vector<vector<double> > A(N,vector<double> (N));
	vector<double> b(N);
	vector<double> x(N);

	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			A[i][j] = rand() % 1000;
		}
		b[i] = rand() % 1000;
	}

	//Print out original Matrix before transformations
	cout << "\n\nOriginal Matrix:" << endl;
	printMatrix(N, A, b);

	//Create an array that can be used to pass the thread_indices to the pthread function
	for(int numThreads = 1; numThreads <= MAX_THREADS; numThreads++){
    	int index[numThreads];

    	for(int i = 0; i < numThreads; i++){
	    	index[i] = i;
	    }
	    startTime = Timer::currentSeconds();
    	for (int i = 0; i < N-1; i++){
			partialPivot(N, A, b, i); //declared in the header file
			passVals.A = &A;
			passVals.b = &b;
			passVals.N = N;
			passVals.i = i;
			passVals.numThreads = numThreads;

			for (int x = 0; x < numThreads; x++){
				pthread_create(&tid[x], NULL, eliminate, &index[x]);
			}

			for (int x = 0; x < numThreads; x++){
				pthread_join(tid[x], NULL);
			}
		}
		endTime = Timer::currentSeconds();
		if(numThreads == 1){
			serialTime = endTime - startTime;
			cout << "\ngaussian serial:\t" << fixed << setprecision(3) << serialTime * 1000 << " ms";
		}
		else{
			cout << "\ngaussian threaded:\t" << fixed << setprecision(3) << (endTime - startTime) * 1000 << " ms\t";
        	cout << "(" << fixed << setprecision(2) << serialTime/(endTime - startTime) << "x speedup from " << numThreads << " threads)";
		}

    }

    cout << endl << endl;
    cout << "Reduced Matrix:" << endl;
    printMatrix(N,A,b); //declared in header file
	backSubstitution(N, A, b, &x); // declared in the header file
	printSolutionVector(x, N); // declared in header file
	cout << endl << endl;
}
