using namespace std;

#ifndef MATRIX_HANDLER_H
#define MATRIX_HANDLER_H

class MatrixHandler {
public:
    MatrixHandler(int initialRows, int initialCols);  // Constructor

    //Basic math
    void transposeMatrix();

    //Other methods
    void printMatrix();
    void fillRandom(int min, int max);

    //Operators overloading
    MatrixHandler operator+(const MatrixHandler& secMatrix) const;
    MatrixHandler operator-(const MatrixHandler& secMatrix) const;
    MatrixHandler operator*(int scalar) const;
    MatrixHandler operator*(const MatrixHandler& otherMatrix) const;

    //Variables
    vector<vector<int>> matrix;

private:
    int rows;
    int cols;

};

#endif // MATRIX_HANDLER_H