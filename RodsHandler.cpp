#include <iostream>
#include "vector"
#include "RodsHandler.h"
#include "matrixHandler.h"

using namespace std;

RodsHandler::RodsHandler(double E, double L, double A) : youngs_Modulus(E), length(L), crossSectionalArea(A) {
	MatrixHandler stiffnessMatrix(2, 2);
	stiffnessMatrix.matrix[0][0] = 1;
	stiffnessMatrix.matrix[0][1] = -1;
	stiffnessMatrix.matrix[1][0] = -1;
	stiffnessMatrix.matrix[1][1] = 1;
	stiffnessMatrix = stiffnessMatrix * ((youngs_Modulus * crossSectionalArea) / length);
	cout << "stiffnessMatrix"<< endl;
	stiffnessMatrix.printMatrix();
	cout << endl;
}

//void RodsHandler::stiffnessMatrixAggregation(vector<vector<int>> K, vector<vector<int>> k, int i, int j) : globalStiffnessMatrix(K),  {
//
//}