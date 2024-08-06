#include "NodeLoad.h"
#include <iostream>

namespace SolverFEM {

NodeLoad::NodeLoad(int nodeNumber, double xLoad, double zLoad, double mLoad)
    : nodeNumber(nodeNumber), xLoad(xLoad), zLoad(zLoad), mLoad(mLoad), loadType(0) {}


double NodeLoad::getXLoad() const {
    return xLoad;
}

double NodeLoad::getZLoad() const {
    return zLoad;
}

double NodeLoad::getMLoad() const {
    return mLoad;
}

int NodeLoad::getNodeNumber() const {
    return nodeNumber;
}

int NodeLoad::getLoadType() const {
    return loadType;
}

} // namespace SolverFEM
