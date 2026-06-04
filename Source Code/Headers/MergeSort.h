#pragma once
#include <vector>
#include <functional>

// ============================================================
//  Generic Merge Sort  —  O(n log n) guaranteed
//  Works on any type T given a comparator: bool(const T&, const T&)
// ============================================================

template <typename T>
static void mergeParts(std::vector<T>& arr, int l, int m, int r,
                       const std::function<bool(const T&, const T&)>& cmp)
{
    std::vector<T> left(arr.begin() + l, arr.begin() + m + 1);
    std::vector<T> right(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;
    while (i < (int)left.size() && j < (int)right.size()) {
        if (cmp(left[i], right[j])) { arr[k++] = left[i++]; }
        else                        { arr[k++] = right[j++]; }
    }
    while (i < (int)left.size())  arr[k++] = left[i++];
    while (j < (int)right.size()) arr[k++] = right[j++];
}

template <typename T>
void mergeSort(std::vector<T>& arr, int l, int r,
               const std::function<bool(const T&, const T&)>& cmp)
{
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m,     cmp);
    mergeSort(arr, m + 1, r, cmp);
    mergeParts(arr, l, m, r, cmp);
}

// Convenience overload: sort the whole vector
template <typename T>
void mergeSort(std::vector<T>& arr,
               const std::function<bool(const T&, const T&)>& cmp)
{
    if (arr.size() > 1)
        mergeSort(arr, 0, (int)arr.size() - 1, cmp);
}
