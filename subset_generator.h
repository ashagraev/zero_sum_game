#pragma once

#include <vector>

// generates all subsets of fixed size using Gray codes, see https://en.wikipedia.org/wiki/Gray_code
class SubsetGenerator {
private:
    const size_t TotalElementsCount;
    const size_t SubsetSize;

    std::vector<bool> Subset;
public:
    SubsetGenerator(const size_t totalElementsCount, const size_t subsetSize)
        : TotalElementsCount(totalElementsCount)
        , SubsetSize(subsetSize)
    {
    }

    template <typename Callback>
    void Run(Callback callback) {
        Subset.resize(TotalElementsCount, false);
        Run(TotalElementsCount, SubsetSize, 0, TotalElementsCount - 1, false, TotalElementsCount, callback);
    }
private:
    template <typename Callback>
    void Run(const int n, const int k, const int l, const int r, const bool rev, const int old_n, Callback callback) {
        if (k > n || k < 0) {
            return;
        }
        if (n == 0) {
            callback(Subset);
            return;
        }

        Subset[rev ? r : l] = false;
        Run(n - 1, k, !rev ? l + 1 : l, !rev ? r : r - 1, rev, old_n, callback);
        Subset[rev ? r : l] = true;
        Run(n - 1, k - 1, !rev ? l + 1 : l, !rev ? r : r - 1, !rev, old_n, callback);
    }
};
