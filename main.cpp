#include "subset_generator.h"
#include "matrix.h"
#include "kkt.h"

#include <iostream>

static inline void PrintVector(const std::vector<double>& values) {
    std::cout.precision(5);

    std::cout << "\t";
    for (const double value : values) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

static inline void PrintSolution(const Solution& solution) {
    std::cout << "\t" << "value: " << solution.GameValue << std::endl;
    PrintVector(solution.FirstMixedStrategy);
    PrintVector(solution.SecondMixedStrategy);
    std::cout << std::endl;
}

int main() {
    std::vector<std::vector<double>> matrix;
    for (size_t caseIdx = 1;; ++caseIdx) {
        matrix = ReadMatrix(std::cin);
        if (matrix.empty()) {
            break;
        }

        const std::vector<Solution> solutions = KKTSolver::Solve(matrix);
        std::cout << "case #" << caseIdx << ":" << std::endl;

        for (const Solution solution : solutions) {
            PrintSolution(solution);
        }
    }
    return 0;
}
