#include "MatrixAggregator.h"
#include "MatrixHandler.h"
using namespace std;


#ifndef RODS_HANDLER_H
#define RODS_HANDLER_H

class RodsHandler : public MatrixAggregator {
public:
    RodsHandler();  // Constructor 
    RodsHandler(double E, double L, double A);  // Constructor 
    

    //Variables
    double youngs_Modulus, length, crossSectionalArea;
    vector<vector<double>> matrix;
    //void stiffnessMatrixAggregation(vector<vector<int>> K, vector<vector<int>> k, int i, int j);

};

#endif // RODS_HANDLER_H