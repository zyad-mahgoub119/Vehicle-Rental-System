#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

// ============================================================
//  Generic Binary Search  —  O(log n)
//
//  PRECONDITION: the vector must already be sorted by the same
//  key you are searching on.  Call mergeSort() first if needed.
// ============================================================


// ---------- Core: returns index or -1 ----------
// keyExtractor : T -> Key
// Returns the index of the first match found, or -1.
template <typename T, typename Key>
int binarySearchIndex(const std::vector<T>& arr,
                      const Key& target,
                      const std::function<Key(const T&)>& keyExtractor)
{
    int lo = 0, hi = (int)arr.size() - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        Key midKey = keyExtractor(arr[mid]);

        if (midKey == target)  return mid;
        else if (midKey < target) lo = mid + 1;
        else                      hi = mid - 1;
    }
    return -1;   // not found
}


// ---------- Search and return pointer to element (or nullptr) ----------
template <typename T, typename Key>
const T* binarySearch(const std::vector<T>& arr,
                      const Key& target,
                      const std::function<Key(const T&)>& keyExtractor)
{
    int idx = binarySearchIndex(arr, target, keyExtractor);
    if (idx == -1) return nullptr;
    return &arr[idx];
}


// ---------- Collect ALL matches (for non-unique keys, e.g. category) ----------
// Uses the found index as a pivot and expands left/right.
template <typename T, typename Key>
std::vector<T> binarySearchAll(const std::vector<T>& arr,
                               const Key& target,
                               const std::function<Key(const T&)>& keyExtractor)
{
    std::vector<T> results;
    int idx = binarySearchIndex(arr, target, keyExtractor);
    if (idx == -1) return results;  // empty

    // Expand left
    int left = idx;
    while (left > 0 && keyExtractor(arr[left - 1]) == target) --left;

    // Expand right
    int right = idx;
    while (right < (int)arr.size() - 1 && keyExtractor(arr[right + 1]) == target) ++right;

    for (int i = left; i <= right; ++i)
        results.push_back(arr[i]);

    return results;
}


// ---------- Range search on a numeric key (e.g. price between min and max) ----------
template <typename T, typename Key>
std::vector<T> binarySearchRange(const std::vector<T>& arr,
                                 const Key& minVal,
                                 const Key& maxVal,
                                 const std::function<Key(const T&)>& keyExtractor)
{
    std::vector<T> results;
    if (arr.empty()) return results;

    // Find first index where key >= minVal
    int lo = 0, hi = (int)arr.size() - 1, start = (int)arr.size();
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (keyExtractor(arr[mid]) >= minVal) { start = mid; hi = mid - 1; }
        else                                   lo = mid + 1;
    }

    // Collect while key <= maxVal
    for (int i = start; i < (int)arr.size(); ++i) {
        if (keyExtractor(arr[i]) > maxVal) break;
        results.push_back(arr[i]);
    }
    return results;
}
