#ifndef MEMBER_H
#define MEMBER_H

namespace SolverFEM {

class Member
{
private:
    int firstNode;  // First Node
    int secondNode; // Second Node
    double x1, z1, x2, z2;
    double length;
    double E, I, A, v; // User Input as GPa, mm^4, mm^2, [-]
    double lambdaZ;    // = z2 - z1 / length
    double lambdaX;    // = x2 - x1 / length
    int lineId;        // Store the line ID associated with this member

public:
    Member(int firstNode,
           int secondNode,
           double x1,
           double z1,
           double x2,
           double z2,
           double E,
           double v,
           double I,
           double A,
           double length,
           int lineId);  // Constructor with lineId

    double getLambdaZ() const;
    double getLambdaX() const;
    double getE() const;
    double getI() const;
    double getA() const;
    double getV() const;
    double getLength() const;
    int getFirstNodeNumber() const;
    int getSecondNodeNumber() const;
    double getX1() const;
    double getZ1() const;
    double getX2() const;
    double getZ2() const;
    int getLineId() const;  // Getter for lineId
};

} // namespace SolverFEM

#endif // MEMBER_H
