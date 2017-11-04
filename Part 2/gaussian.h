#ifndef GAUSSIAN_H_
#define GAUSSIAN_H_
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
using namespace std;

void partialPivot(int n, vector<vector<double> > a, vector<double> b, int j){

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

void printMatrix(int N, vector<vector<double> > A, vector<double> b){
	for (int x=0; x<N; x++){
		cout << "| ";
		for(int y=0; y<N; y++)
			cout << left << fixed << setprecision(3) << setw(13) <<  A[x][y];
		cout << "| | " << left << fixed << setprecision(3) << setw(9) << b[x] << " |\n";
	}
}

void printSolutionVector(vector<double> x, int N){
	cout << "\nSolution Vector x:\n";
	for (int i=0; i<N; i++){
		cout << "|" << left << fixed << setprecision(3) << setw(7) << x[i] << "|\n";
	}
}

#endif