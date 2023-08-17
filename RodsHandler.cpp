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
	double helpful = ((youngs_Modulus * crossSectionalArea) / length);
	cout << helpful << endl;
	stiffnessMatrix = stiffnessMatrix * helpful;
	cout << "stiffnessMatrix"<< endl;
	stiffnessMatrix.printMatrix();
	cout << endl;
}
