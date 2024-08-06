#ifndef MATRIXOPERATIONS_H
#define MATRIXOPERATIONS_H

#include <Eigen/Dense>
#include <vector>
#include <string>

namespace SolverFEM {

class MatrixOperations {
public:
    // Matrix multiplication
    static Eigen::MatrixXd multiply(const Eigen::MatrixXd& M1, const Eigen::MatrixXd& M2);

    // Matrix addition
    static Eigen::MatrixXd add(const Eigen::MatrixXd& M1, const Eigen::MatrixXd& M2);

    // Matrix subtraction
    static Eigen::MatrixXd subtract(const Eigen::MatrixXd& M1, const Eigen::MatrixXd& M2);

    // Matrix inversion
    static Eigen::MatrixXd inverse(const Eigen::MatrixXd& M);

    // Build identity matrix
    static Eigen::MatrixXd identity(int size);

    // Print matrix
    static void print(const Eigen::MatrixXd& M);

    // Reduce matrix based on index and direction
    static Eigen::MatrixXd reduce(const Eigen::MatrixXd& M, const std::vector<int>& indices, const std::string& direction);
};

} // namespace SolverFEM

#endif // MATRIXOPERATIONS_H
