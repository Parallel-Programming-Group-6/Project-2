// header file to perform row reduction echelon form of matrix using Gaussian's Method

#ifndef GAUSSIAN_H_
#define GAUSSIAN_H_
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
using namespace std;

// partial pivot between rows 
void partialPivot(int z, vector<vector<double> > a, vector<double> b, int j){

	int i;
	int k;
	int m;
	int rowx;
	double xfac;
	double temp;
	double temp1;
	double amax;

	amax = (double) fabs(a[j][j]) ;
    m = j;
    for (i=j+1; i<z; i++){   /* Find the row with largest pivot */
    	xfac = (double) fabs(a[i][j]);
    	if(xfac > amax) {amax = xfac; m=i;}
    }

    if(m != j) {  /* Row interchanges */
    	rowx = rowx+1;
    	temp1 = b[j];
    	b[j]  = b[m];
    	b[m]  = temp1;
    	for(k=j; k<z; k++) {
    		temp = a[j][k];
    		a[j][k] = a[m][k];
    		a[m][k] = temp;
    	}
    }
}

//back-substitution for matrix 
void backSubstitution(int N, vector<vector<double> > A, vector<double> b, vector<double> *x1){
	vector<double> &x = *x1;
	for (int i = N-1; i >= 0; i--){
		x[i] = b[i];
		for (int j = i + 1; j < N; j++){
			x[i] -= A[i][j]*x[j];
		}
		x[i] = x[i] / A[i][i];
	}
}

//Print out the matrix
void printMatrix(int Index, vector<vector<double> > A, vector<double> b){
	for (int x=0; x<Index; x++){
		cout << "| ";
		for(int y=0; y<Index; y++)
			cout << left << fixed << setprecision(3) << setw(13) <<  A[x][y];
		cout << "| | " << left << fixed << setprecision(3) << setw(9) << b[x] << " |\n";
	}
}

//Print out solution vector b
void printSolutionVector(vector<double> x, int Index){
	cout << "\nSolution Vector x:\n";
	for (int i=0; i<Index; i++){
		cout << "|" << left << fixed << setprecision(3) << setw(7) << x[i] << "|\n";
	}
}

#endif
