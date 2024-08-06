#ifndef NODESUPPORT_H
#define NODESUPPORT_H

#include <string>

namespace SolverFEM {

class NodeSupport {
private:
    std::string supportType;
    int d1, d2, d3; // d1 corresponds to x, d2 to y, d3 to z

    void updateConstraints(const std::string &supportType);

public:
    NodeSupport(const std::string &supportType);

    void setSupport(const std::string &supportType);
    void deleteSupport();

    std::string getSupportType() const;
    int getD1() const;
    int getD2() const;
    int getD3() const;
};

} // namespace SolverFEM

#endif // NODESUPPORT_H
