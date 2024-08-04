//
// Created by Julian Haudek on 15/02/2024.
//

#ifndef MASTERTHESIS_CROSSSECTIONSASSISTANT_H
#define MASTERTHESIS_CROSSSECTIONSASSISTANT_H


class CrossSectionsAssistant {

public :
    double calculateArea(int b, int h);
    double calculateInertia(int b, int h);
    double calculateArea(int r);
    double calculateInertia(int r);
};


#endif //MASTERTHESIS_CROSSSECTIONSASSISTANT_H
