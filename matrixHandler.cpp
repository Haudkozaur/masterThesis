#include <iostream>
#include "vector"
#include "matrixHandler.h"
#include "random"
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

//MatrixHandler class implementation using eigen

MatrixHandler::MatrixHandler() : rows(0), cols(0), matrix() {
}

MatrixHandler::MatrixHandler(int initialRows, int initialCols) : rows(initialRows), cols(initialCols),
                                                                 matrix(initialRows, vector<double>(initialCols)) {
    vector<vector<double>> matrix(rows, vector<double>(cols));
}

//Transposing
void MatrixHandler::transposeMatrix() {
    vector<vector<double>> transposedMatrix(cols, vector<double>(rows));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; j++) {
            transposedMatrix[j][i] = matrix[i][j];
        }
    }
    matrix = transposedMatrix;
}
//Print object-matrix 

void MatrixHandler::printMatrix() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << matrix[i][j] << endl;
        }

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


//Multiply matrix by vector, edit matrix in place
void MatrixHandler::multiplyByVector(vector<double> vector) {
    if (rows == vector.size()) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                matrix[i][j] *= vector[i];
            }
        }
        cout << "Matrix multiplied" << endl;
    } else {
        cout << "Matrix and vector sizes are incompatible for multiplication";
        cout << endl;
    }
}
vector<double> MatrixHandler::solveLinearSystem(vector<double> vector) {
    std::vector<double> result;
    if (rows == cols and rows == vector.size()) {
        Eigen::MatrixXd A(rows, cols);
        Eigen::VectorXd b(rows);
        Eigen::VectorXd x(cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                A(i, j) = matrix[i][j];
            }
            b(i) = vector[i];
        }
        x = A.colPivHouseholderQr().solve(b);
        for (int i = 0; i < cols; ++i) {
            result.push_back(x(i));
        }
        cout << "Linear system solved" << endl;
    } else {
        cout << "Matrix and vector sizes are incompatible for solving linear system";
        cout << endl;
    }
    return result;
}







//Operators overloading

MatrixHandler MatrixHandler::operator+(const MatrixHandler &secMatrix) const {
    if (rows == secMatrix.rows and cols == secMatrix.cols) {
        cout << "Matrices can be added";
        cout << endl;
        MatrixHandler result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.matrix[i][j] = matrix[i][j] + secMatrix.matrix[i][j];
            }
        }
        return result;
    } else {
        cout << "Matrices sizes are incompatible for addition";
        cout << endl;
        return *this;

    }
}

MatrixHandler MatrixHandler::operator-(const MatrixHandler &secMatrix) const {
    if (rows == secMatrix.rows and cols == secMatrix.cols) {
        cout << "Matrices can be subtract";
        cout << endl;
        MatrixHandler result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.matrix[i][j] = matrix[i][j] - secMatrix.matrix[i][j];
            }
        }
        return result;
    } else {
        cout << "Matrices sizes are incompatible for subtraction";
        cout << endl;
        return *this;

    }
}

MatrixHandler MatrixHandler::operator*(double scalar) const {
    MatrixHandler result(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result.matrix[i][j] = matrix[i][j] * scalar;
        }
    }
    cout << "Matrix multiplied" << endl;

    return result;
}

MatrixHandler MatrixHandler::operator*(const MatrixHandler &secMatrix) const {
    if (cols == secMatrix.matrix.size()) {
        cout << "Matrices can be multiplied";
        cout << endl;
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
    } else {
        cout << "Matrices sizes are incompatible for multiplication";
        cout << endl;
        return *this;
    }
}
