#pragma once

#include "matrix.h"

#include <vector>

struct Solution {
    std::vector<double> FirstMixedStrategy;
    std::vector<double> SecondMixedStrategy;

    double GameValue = 0.;

    Solution() {
    }

    Solution(const size_t variablesCount)
        : FirstMixedStrategy(variablesCount)
        , SecondMixedStrategy(variablesCount)
    {
    }
};

class KKTSolver {
public:
    static std::vector<Solution> Solve(const std::vector<std::vector<double>>& matrix, const double eps = 1e-5);
};
