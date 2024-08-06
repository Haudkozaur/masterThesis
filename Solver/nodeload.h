#ifndef NODELOAD_H
#define NODELOAD_H

namespace SolverFEM {

class NodeLoad
{
private:
    int nodeNumber;
    double xLoad; // X Axis
    double zLoad; // Z Axis
    double mLoad; // Moment
    int loadType;

public:
    NodeLoad(int nodeNumber, double xLoad, double zLoad, double mLoad);

    double getXLoad() const;
    double getZLoad() const;
    double getMLoad() const;
    int getNodeNumber() const;
    int getLoadType() const;
};

} // namespace SolverFEM

#endif // NODELOAD_H
