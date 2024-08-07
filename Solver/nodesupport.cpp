#include "NodeSupport.h"

namespace SolverFEM {

// Constructor to initialize support constraints and IDs
NodeSupport::NodeSupport(int id, int nodeId, bool Tx, bool Tz, bool Ry)
    : id(id), nodeId(nodeId), Tx(Tx), Tz(Tz), Ry(Ry) {}

// Getters
int NodeSupport::getId() const {
    return id;
}

int NodeSupport::getNodeId() const {
    return nodeId;
}

bool NodeSupport::getTx() const {
    return Tx;
}

bool NodeSupport::getTz() const {
    return Tz;
}

bool NodeSupport::getRy() const {
    return Ry;
}

} // namespace SolverFEM
