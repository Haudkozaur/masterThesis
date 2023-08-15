// matrixHandler.cpp
#include <iostream>
#include "vector"
#include "matrixHandler.h"
#include "random"
using namespace std;

MatrixHandler::MatrixHandler(int initialRows, int initialCols) : rows(initialRows), cols(initialCols), matrix(initialRows, vector<int>(initialCols)) {
    vector<vector<int>> matrix(rows, vector<int>(cols));
}

//Tranposing
void MatrixHandler::transposeMatrix() {
    vector<vector<int>> transposedMatrix(cols, vector<int>(rows));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposedMatrix[j][i] = matrix[i][j];
        }
    }
}

//Print object-matrix

void MatrixHandler::printMatrix() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << matrix[i][j] << "\t";
        }
        cout << "\n";
    }
}
//Create random matrix for tests

void MatrixHandler::fillRandom(int min, int max) {

    // Create a random number generator engine
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(min, max);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dist(gen);
        }
    }
}

//Operators overloading

MatrixHandler MatrixHandler::operator+(const MatrixHandler& secMatrix) const {
    if (rows == secMatrix.rows and cols == secMatrix.cols) {
        cout << "Matrices can be added";
        cout << endl;
        MatrixHandler result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.matrix[i][j] = matrix[i][j] + secMatrix.matrix[i][j];
            }
        } return result;
    }
    else {
        cout << "Matrices sizes are incompatible for addition";
        cout << endl;
        return *this;

    }
}
MatrixHandler MatrixHandler::operator-(const MatrixHandler& secMatrix) const {
    if (rows == secMatrix.rows and cols == secMatrix.cols) {
        cout << "Matrices can be subtract";
        cout << endl;
        MatrixHandler result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.matrix[i][j] = matrix[i][j] - secMatrix.matrix[i][j];
            }
        } return result;
    }
    else {
        cout << "Matrices sizes are incompatible for subtraction";
        cout << endl;
        return *this;

    }
}
MatrixHandler MatrixHandler::operator*(int scalar) const {
    MatrixHandler result(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result.matrix[i][j] = matrix[i][j] * scalar;
        }
    }

    return result;
}
MatrixHandler MatrixHandler::operator*(const MatrixHandler& secMatrix) const {
    if (cols == secMatrix.matrix.size()) {
        MatrixHandler result(rows, secMatrix.cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < secMatrix.cols; ++j) {
                result.matrix[i][j] = 0;
                for (int k = 0; k < cols; ++k) {
                    result.matrix[i][j] += matrix[i][k] * secMatrix.matrix[k][j];
                }
            }
        }

        return result;
    }
    else {
        cout << "Matrices sizes are incompatible for multiplication";
        cout << endl;
        return *this;
    }
}
