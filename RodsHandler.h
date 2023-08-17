using namespace std;

#ifndef RODS_HANDLER_H
#define RODS_HANDLER_H

class RodsHandler {
public:
    RodsHandler(double E, double L, double A);  // Constructor 
    

    //Variables
    double youngs_Modulus, length, crossSectionalArea;
    vector<vector<int>> stiffnessMatrix;
    
    

};

#endif // RODS_HANDLER_H