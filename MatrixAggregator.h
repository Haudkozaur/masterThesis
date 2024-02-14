using namespace std;
#include <vector>
#include "matrixHandler.h"

#ifndef MATRIX_AGGREGATOR_H
#define MATRIX_AGGREGATOR_H

class MatrixAggregator {
public:

	//Aggregations of Global Stiffnes Matrices for different types of elements

	virtual void rodsStiffnessMatrixAggregation(vector<vector<double>> K, vector<vector<double>> k, int i, int j);

	//Variables 
	vector<vector<double>> globalStiffnesMatrixAggregated;
};

#endif // MATRIX_AGGREGATOR_H