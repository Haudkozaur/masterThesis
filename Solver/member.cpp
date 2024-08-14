#include "Member.h"
#include <cmath>

namespace SolverFEM {

Member::Member(int firstNode,
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
               int lineId)  // Add lineId here
    : firstNode(firstNode)
    , secondNode(secondNode)
    , x1(x1)
    , z1(z1)
    , x2(x2)
    , z2(z2)
    , E(E)
    , v(v)
    , I(I)
    , A(A)
    , length(length)
    , lineId(lineId)  // Initialize lineId
{
    lambdaZ = (z2 - z1) / length;
    lambdaX = (x2 - x1) / length;
}

int Member::getLineId() const
{
    return lineId;
}

double Member::getLambdaZ() const
{
    return lambdaZ;
}

double Member::getLambdaX() const
{
    return lambdaX;
}

double Member::getE() const
{
    return E;
}

double Member::getI() const
{
    return I;
}

double Member::getA() const
{
    return A;
}

double Member::getV() const
{
    return v;
}

double Member::getLength() const
{
    return length;
}

int Member::getFirstNodeNumber() const
{
    return firstNode;
}

int Member::getSecondNodeNumber() const
{
    return secondNode;
}

double SolverFEM::Member::getX1() const
{
    return x1;
}

double SolverFEM::Member::getZ1() const
{
    return z1;
}

double SolverFEM::Member::getX2() const
{
    return x2;
}

double SolverFEM::Member::getZ2() const
{
    return z2;
}

} // namespace SolverFEM
