#include "NodeSupport.h"

namespace SolverFEM {

NodeSupport::NodeSupport(const std::string &supportType)
    : supportType(supportType) {
    updateConstraints(supportType);
}

void NodeSupport::updateConstraints(const std::string &supportType) {
    if (supportType == "xyz") { d1 = 0; d2 = 0; d3 = 0; }
    else if (supportType == "xz") { d1 = 0; d2 = 1; d3 = 0; } // Adjusted for xz plane
    else if (supportType == "yz") { d1 = 1; d2 = 0; d3 = 0; } // Adjusted for yz plane
    else if (supportType == "x") { d1 = 0; d2 = 1; d3 = 1; }
    else if (supportType == "z") { d1 = 1; d2 = 1; d3 = 0; }
    else if (supportType == "y") { d1 = 1; d2 = 0; d3 = 1; }
    else if (supportType == "free") { d1 = 1; d2 = 1; d3 = 1; }
    else if (supportType == "ixyz") { d1 = 1; d2 = 1; d3 = 1; }
    else { d1 = 0; d2 = 0; d3 = 0; }
}

void NodeSupport::setSupport(const std::string &supportType) {
    this->supportType = supportType;
    updateConstraints(supportType);
}

void NodeSupport::deleteSupport() {
    supportType = "free";
    d1 = 1;
    d2 = 1;
    d3 = 1;
}

std::string NodeSupport::getSupportType() const {
    return supportType;
}

int NodeSupport::getD1() const {
    return d1;
}

int NodeSupport::getD2() const {
    return d2;
}

int NodeSupport::getD3() const {
    return d3;
}

} // namespace SolverFEM
