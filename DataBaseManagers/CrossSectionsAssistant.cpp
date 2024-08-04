#include <cmath>
#include "CrossSectionsAssistant.h"
#include <iostream>

//For rectangular cross-sections
double CrossSectionsAssistant::calculateArea(int b, int h) {
    double area = b * h;
    area = area/1000000;
    std::cout << area << std::endl;
    return area;

}

double CrossSectionsAssistant::calculateInertia(int b, int h) {
    double inertia = (b * pow(h, 3)) / 12;
    inertia = inertia/1000000000000;
    std::cout << inertia << std::endl;;
    return inertia;
}

//For circular cross-sections
double CrossSectionsAssistant::calculateArea(int r) {
    double area = M_PI * pow(r, 2);
    area = area/1000000;
    return area;
}

double CrossSectionsAssistant::calculateInertia(int r) {
    double inertia = (M_PI * pow(r, 4)) / 4;
    inertia = inertia/1000000000000;
    return inertia;
}
