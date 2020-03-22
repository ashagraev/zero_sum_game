#include "kkt.h"

#include "subset_generator.h"

namespace {
    bool AllElementsArePositive(const std::vector<double>& sequence) {
        for (const double e : sequence) {
            if (e < 0) {
                return false;
            }
        }
        return true;
    }

    size_t PositiveCount(const std::vector<double>& sequence) {
        size_t positiveCount = 0;
        for (const double e : sequence) {
            positiveCount += e > 0;
        }
        return positiveCount;
    }

    bool IsGoodMixedStrategy(const std::vector<double>& sequence, const size_t targetPositivesCount) {
        return AllElementsArePositive(sequence) && PositiveCount(sequence) == targetPositivesCount;
    }

    bool IsGoodSolution(const Solution& solution, const size_t targetPositivesCount) {
        return IsGoodMixedStrategy(solution.FirstMixedStrategy, targetPositivesCount) &&
               IsGoodMixedStrategy(solution.SecondMixedStrategy, targetPositivesCount);
    }

    std::vector<std::vector<double>> PrepareSLAEMatrix(const std::vector<std::vector<double>>& source,
                                                       const std::vector<bool>& rowsToUse,
                                                       const std::vector<bool>& colsToUse)
    {
        std::vector<std::vector<double>> cleanMatrix = RemoveUseless(source, rowsToUse, colsToUse);
        for (std::vector<double>& cleanRow : cleanMatrix) {
            cleanRow.push_back(1.);
        }
        cleanMatrix.push_back(std::vector<double>(cleanMatrix.size() + 1, 1.));
        cleanMatrix.back().back() = 0.;
        return cleanMatrix;
    }


    std::vector<double> PrepareRight(const std::vector<std::vector<double>>& preparedMatix) {
        std::vector<double> right(preparedMatix.size());
        right.back() = 1.;
        return right;
    }

    std::vector<double> RestoreArray(const std::vector<double>& sequence, const std::vector<bool>& used) {
        std::vector<double> result(used.size());
        size_t pointer = 0;
        for (size_t i = 0; i < used.size(); ++i) {
            if (!used[i]) {
                continue;
            }
            result[i] = sequence[pointer++];
        }
        return result;
    }

    double GameValue(const std::vector<std::vector<double>>& matrix, const Solution& solution)
    {
        double value = 0.;
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix[i].size(); ++j) {
                value += matrix[i][j] * solution.FirstMixedStrategy[i] * solution.SecondMixedStrategy[j];
            }
        }
        return value;
    }

    std::vector<Solution> FilterNonEquilibrium(const std::vector<std::vector<double>>& matrix, const std::vector<Solution>& solutions, const double eps) {
        std::vector<Solution> equilibriums;

        for (const Solution solution : solutions) {
            bool isEquilibrium = true;

            Solution firstChanged(solution);
            Solution secondChanged(solution);
            for (const Solution otherSolution : solutions) {
                firstChanged.FirstMixedStrategy = otherSolution.FirstMixedStrategy;
                secondChanged.SecondMixedStrategy = otherSolution.SecondMixedStrategy;

                const double firstChangedGameValue = GameValue(matrix, firstChanged);
                const double secondChangedGameValue = GameValue(matrix, secondChanged);

                if (firstChangedGameValue > solution.GameValue + eps || secondChangedGameValue + eps < solution.GameValue) {
                    isEquilibrium = false;
                    break;
                }
            }

            if (isEquilibrium) {
                equilibriums.push_back(solution);
            }
        }

        return equilibriums;
    }
}

std::vector<Solution> KKTSolver::Solve(const std::vector<std::vector<double>>& matrix, const double eps) {
    std::vector<Solution> solutions;

    const size_t firstPlayerStrategiesCount = matrix.size();
    const size_t secondPlayerStrategiesCount = matrix.front().size();

    for (size_t variablesToUse = 1; variablesToUse <= firstPlayerStrategiesCount; ++variablesToUse) {
        SubsetGenerator firstGenerator(firstPlayerStrategiesCount, variablesToUse);
        firstGenerator.Run([&](const std::vector<bool>& firstSubset) {
            SubsetGenerator secondGenerator(secondPlayerStrategiesCount, variablesToUse);
            secondGenerator.Run([&](const std::vector<bool>& secondSubset) {
                const std::vector<std::vector<double>> qMatrix = PrepareSLAEMatrix(matrix, firstSubset, secondSubset);
                const std::vector<std::vector<double>> pMatrix = Transpose(qMatrix);
                const std::vector<double> right = PrepareRight(qMatrix);

                Solution solution;
                solution.FirstMixedStrategy = ::Solve(pMatrix, right, eps);
                solution.FirstMixedStrategy = RestoreArray(solution.FirstMixedStrategy, firstSubset);

                solution.SecondMixedStrategy = ::Solve(qMatrix, right, eps);
                solution.SecondMixedStrategy = RestoreArray(solution.SecondMixedStrategy, secondSubset);

                solution.GameValue = GameValue(matrix, solution);

                if (IsGoodSolution(solution, variablesToUse)) {
                    solutions.push_back(solution);
                }
                });
            });
    }

    return FilterNonEquilibrium(matrix, solutions, eps);
}
