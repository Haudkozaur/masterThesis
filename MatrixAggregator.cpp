#include <iostream>
#include "vector"
#include "MatrixAggregator.h"
#include "matrixHandler.h"

using namespace std;

void MatrixAggregator::rodsStiffnessMatrixAggregation(vector<vector<double>> K, vector<vector<double>> k, int i, int j) {
    K[i][i] += k[0][0];
    K[i][j] += k[0][1];
    K[j][i] += k[1][0];
    K[j][j] += k[1][1];
    cout << K.size() << " rozmiar K" << endl;
    cout << k.size() << " rozmiar malego k" << endl;
    globalStiffnesMatrixAggregated = K;

}


