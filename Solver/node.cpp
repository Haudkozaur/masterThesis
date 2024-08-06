#include "node.h"
namespace SolverFEM {

Node::Node(int id, double x, double z)
    : id(id)
    , x(x)
    , z(z)
    , d1(0.0)
    , d2(0.0)
    , d3(0.0)
{}
double Node::getX() const
{
    return x;
}

double Node::getZ() const
{
    return z;
}

double Node::getD1() const
{
    return d1;
}

double Node::getD2() const
{
    return d2;
}

double Node::getD3() const
{
    return d3;
}

int Node::getId() const
{
    return id;
}

void Node::setDisplacements(double newD1, double newD2, double newD3)
{
    d1 = newD1;
    d2 = newD2;
    d3 = newD3;
}

} // namespace SolverFEM
