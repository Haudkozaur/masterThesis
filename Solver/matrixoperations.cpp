#include "MatrixOperations.h"
#include <iostream>

namespace SolverFEM {

// Matrix multiplication
Eigen::MatrixXd MatrixOperations::multiply(const Eigen::MatrixXd& M1, const Eigen::MatrixXd& M2) {
    return M1 * M2;
}

// Matrix addition
Eigen::MatrixXd MatrixOperations::add(const Eigen::MatrixXd& M1, const Eigen::MatrixXd& M2) {
    return M1 + M2;
}

// Matrix subtraction
Eigen::MatrixXd MatrixOperations::subtract(const Eigen::MatrixXd& M1, const Eigen::MatrixXd& M2) {
    return M1 - M2;
}

// Matrix inversion
Eigen::MatrixXd MatrixOperations::inverse(const Eigen::MatrixXd& M) {
    return M.inverse();
}

// Build identity matrix
Eigen::MatrixXd MatrixOperations::identity(int size) {
    return Eigen::MatrixXd::Identity(size, size);
}

// Print matrix
void MatrixOperations::print(const Eigen::MatrixXd& M) {
    std::cout << M << std::endl;
}

// Reduce matrix based on index and direction
Eigen::MatrixXd MatrixOperations::reduce(const Eigen::MatrixXd& M, const std::vector<int>& indices, const std::string& direction) {
    Eigen::MatrixXd reducedMatrix;
    std::vector<int> rowsToKeep, colsToKeep;

    for (int i = 0; i < M.rows(); ++i) {
        if (std::find(indices.begin(), indices.end(), i) == indices.end()) {
            rowsToKeep.push_back(i);
        }
    }

    for (int j = 0; j < M.cols(); ++j) {
        if (std::find(indices.begin(), indices.end(), j) == indices.end()) {
            colsToKeep.push_back(j);
        }
    }

    if (direction == "both") {
        reducedMatrix.resize(rowsToKeep.size(), colsToKeep.size());
        for (int i = 0; i < rowsToKeep.size(); ++i) {
            for (int j = 0; j < colsToKeep.size(); ++j) {
                reducedMatrix(i, j) = M(rowsToKeep[i], colsToKeep[j]);
            }
        }
    } else if (direction == "raw") {
        reducedMatrix.resize(rowsToKeep.size(), M.cols());
        for (int i = 0; i < rowsToKeep.size(); ++i) {
            reducedMatrix.row(i) = M.row(rowsToKeep[i]);
        }
    } else if (direction == "col") {
        reducedMatrix.resize(M.rows(), colsToKeep.size());
        for (int j = 0; j < colsToKeep.size(); ++j) {
            reducedMatrix.col(j) = M.col(colsToKeep[j]);
        }
    }

    return reducedMatrix;
}

} // namespace SolverFEM
