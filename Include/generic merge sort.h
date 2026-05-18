#pragma once
#include <vector>
#include <functional>
using namespace std;
class Sorter {
public:

template<typename T>
    static void mergeSort(vector<T>& arr,function<bool(const T&, const T&)> comparator) {
        if (arr.size() <= 1) return;
        mergeSortHelper(arr, 0, (int)arr.size() - 1, comparator);
    }

private:

    template<typename T>
    static void mergeSortHelper(vector<T>& arr, int l, int r,function<bool(const T&, const T&)>& comparator) {
        if (l < r) {
            int m = l + (r - l) / 2;
            mergeSortHelper(arr, l, m, comparator);
            mergeSortHelper(arr, m + 1, r, comparator);
            merge(arr, l, m, r, comparator);
        }
    }

    template<typename T>
    static void merge(vector<T>& arr, int l, int m, int r,
                      function<bool(const T&, const T&)>& comparator) {
        int nL = m - l + 1;
        int nR = r - m;

        vector<T> L(arr.begin() + l, arr.begin() + l + nL);
        vector<T> R(arr.begin() + m + 1, arr.begin() + m + 1 + nR);

        int i = 0, j = 0, k = l;
        while (i < nL && j < nR) {
            if (comparator(L[i], R[j])) arr[k++] = L[i++];
            else                         arr[k++] = R[j++];
        }
        while (i < nL) arr[k++] = L[i++];
        while (j < nR) arr[k++] = R[j++];
    }
};