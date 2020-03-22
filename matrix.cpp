#include "matrix.h"

std::vector<std::vector<double>> ReadMatrix(std::istream& input) {
    if (input.eof()) {
        return std::vector<std::vector<double>>();
    }

    size_t rows, cols;
    input >> rows >> cols;

    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            input >> matrix[i][j];
        }
    }
    return matrix;
}

std::vector<std::vector<double>> Transpose(const std::vector<std::vector<double>>& sourceMatrix) {
    if (sourceMatrix.empty()) {
        return std::vector<std::vector<double>>();
    }

    const size_t rowsCount = sourceMatrix.size();
    const size_t colsCount = sourceMatrix.front().size();

    std::vector<std::vector<double>> transposedMatrix(colsCount, std::vector<double>(rowsCount));
    for (size_t i = 0; i < rowsCount; ++i) {
        for (size_t j = 0; j < colsCount; ++j) {
            transposedMatrix[j][i] = sourceMatrix[i][j];
        }
    }
    return transposedMatrix;
}

std::vector<double> Solve(const std::vector<std::vector<double>>& matrix, const std::vector<double>& right, const double eps) {
    std::vector<std::vector<double>> mutableMatrix(matrix);
    std::vector<double> mutableRight(right);

    const size_t rows = mutableMatrix.size();
    const size_t cols = mutableMatrix.front().size();

    std::vector<size_t> permutation(cols, (size_t) -1);

    for (size_t row = 0, col = 0; col < cols && row < rows; ++col) {
        size_t primeRow = row;
        for (size_t i = row; i < rows; ++i) {
            if (abs(mutableMatrix[i][col]) > abs(mutableMatrix[primeRow][col])) {
                primeRow = i;
            }
        }

        if (abs(mutableMatrix[primeRow][col]) < eps) {
            continue;
        }

        std::swap(mutableMatrix[primeRow], mutableMatrix[row]);
        std::swap(mutableRight[primeRow], mutableRight[row]);
        permutation[col] = row;

        for (size_t i = 0; i < rows; ++i) {
            if (i != row) {
                const double buffer = mutableMatrix[i][col] / mutableMatrix[row][col];
                for (size_t j = col; j < cols; ++j) {
                    mutableMatrix[i][j] -= mutableMatrix[row][j] * buffer;
                }
                mutableRight[i] -= mutableRight[row] * buffer;
            }
        }

        ++row;
    }

    std::vector<double> solution(cols);
    for (size_t i = 0; i < cols; ++i) {
        if (permutation[i] != -1) {
            solution[i] = mutableRight[permutation[i]] / mutableMatrix[permutation[i]][i];
        }
    }

    return solution;
}

std::vector<std::vector<double>> RemoveUseless(const std::vector<std::vector<double>>& sourceMatrix, const std::vector<bool>& rowsToUse, const std::vector<bool>& colsToUse) {
    std::vector<std::vector<double>> cleanMatrix;

    for (size_t i = 0; i < sourceMatrix.size(); ++i) {
        if (!rowsToUse[i]) {
            continue;
        }

        const std::vector<double>& sourceRow = sourceMatrix[i];
        std::vector<double> cleanRow;
        for (size_t j = 0; j < sourceRow.size(); ++j) {
            if (!colsToUse[j]) {
                continue;
            }
            cleanRow.push_back(sourceRow[j]);
        }
        cleanMatrix.push_back(cleanRow);
    }

    return cleanMatrix;
}
