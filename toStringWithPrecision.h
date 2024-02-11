#include "iostream"
#include "sstream"

#ifndef MASTERTHESIS_TOSTRINGWITHPRECISION_H
#define MASTERTHESIS_TOSTRINGWITHPRECISION_H


using namespace std;

inline string toStringWithPrecision(double value) {
    stringstream stream;
    stream << value;
    return stream.str();
}

#endif //MASTERTHESIS_TOSTRINGWITHPRECISION_H
