#include "uniformload.h"

namespace SolverFEM {

UniformLoad::UniformLoad(int id, int memberId, double Fx, double Fz)
    : id(id), memberId(memberId), Fx(Fx), Fz(Fz)
{}

int UniformLoad::getId() const
{
    return id;
}

int UniformLoad::getMemberId() const
{
    return memberId;
}

double UniformLoad::getFx() const
{
    return Fx;
}

double UniformLoad::getFz() const
{
    return Fz;
}

} // namespace SolverFEM
