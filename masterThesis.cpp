#include <iostream>
#include <vector>
#include "matrixHandler.h"

using namespace std;

int main()
{
    cout << "Hello World!\n";
    int rows, cols;

    // Get the size of the matrix
    cout << "Enter the number of rows: ";
    cin >> rows;

    cout << "Enter the number of columns: ";
    cin >> cols;

    // Create a vector of vectors to create the matrix
    MatrixHandler testMatrix(rows, cols);

    // Print the matrix 
    testMatrix.printMatrix();
    testMatrix.matrix[1][1] = 69;
    cout << endl;
    testMatrix.printMatrix();
    cout << endl;
    MatrixHandler secMatrix(3, 3);

    testMatrix.fillRandom(-10, 10);
    testMatrix.printMatrix();
    cout << endl;
    secMatrix.fillRandom(-10, 10);
    secMatrix.printMatrix();
    cout << endl;

    testMatrix.printMatrix();
    cout << endl;
    secMatrix = secMatrix * testMatrix;
    secMatrix.printMatrix();
    cout << endl;

}

