#ifndef UNIFORMLOAD_H
#define UNIFORMLOAD_H

namespace SolverFEM {

class UniformLoad
{
private:
    const int id;             // ID of the load
    const int memberId;       // ID of the assigned member
    const double Fx;          // Uniform load in the X direction
    const double Fz;          // Uniform load in the Z direction

public:
    UniformLoad(int id, int memberId, double Fx, double Fz);

    int getId() const;
    int getMemberId() const;
    double getFx() const;
    double getFz() const;
};

} // namespace SolverFEM

#endif // UNIFORMLOAD_H
