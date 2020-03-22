#pragma once

#include <vector>
#include <iostream>

std::vector<std::vector<double>> ReadMatrix(std::istream& input);

std::vector<double> Solve(const std::vector<std::vector<double>>& matrix, const std::vector<double>& right, const double eps = 1e-5);
std::vector<std::vector<double>> Transpose(const std::vector<std::vector<double>>& sourceMatrix);
std::vector<std::vector<double>> RemoveUseless(const std::vector<std::vector<double>>& sourceMatrix, const std::vector<bool>& rowsToUse, const std::vector<bool>& colsToUse);
