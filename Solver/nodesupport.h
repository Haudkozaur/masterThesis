#ifndef NODESUPPORT_H
#define NODESUPPORT_H

namespace SolverFEM {

class NodeSupport {
private:
    int id;       // Unique ID for the support
    int nodeId;   // Node ID associated with the support
    bool Tx;      // Constraint on displacement in the x-direction
    bool Tz;      // Constraint on displacement in the z-direction
    bool Ry;      // Constraint on rotation about the y-axis

public:
    // Constructor to initialize support constraints and IDs
    NodeSupport(int id, int nodeId, bool Tx, bool Tz, bool Ry);

    // Getters
    int getId() const;
    int getNodeId() const;
    bool getTx() const;
    bool getTz() const;
    bool getRy() const;
};

} // namespace SolverFEM

#endif // NODESUPPORT_H
