#ifndef NODE_H
#define NODE_H

namespace SolverFEM {

class Node
{
private:
    int id;
    double x;
    double z;
    double d1;
    double d2;
    double d3; // Displacements

public:
    Node(int id, double x, double z);
    void setDisplacements(double newD1, double newD2, double newD3);
    int getId() const;
    double getX() const;
    double getZ() const;
    double getD1() const;
    double getD2() const;
    double getD3() const;
};
} // namespace SolverFEM

#endif // NODE_H
