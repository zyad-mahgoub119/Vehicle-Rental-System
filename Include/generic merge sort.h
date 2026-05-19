#include <iostream>
#include <functional>
using namespace std;

template <typename T>
void merge(T arr[], int l, int m, int r, function<bool(const T&, const T&)> cmp)
{
    int nL = m - l + 1;
    int nR = r - m;

    T* L = new T[nL];
    T* R = new T[nR];

    for (int i = 0; i < nL; i++) L[i] = arr[l + i];
    for (int j = 0; j < nR; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < nL && j < nR) {
        if (cmp(L[i], R[j])) { arr[k] = L[i]; i++; }
        else
          { arr[k] = R[j];
            j++;
          }
        k++;
    }
    while (i < nL) {
        arr[k] = L[i];
        i++;
        k++; }
    while (j < nR) {
        arr[k] = R[j];
        j++;
        k++; }

    delete[] L;
    delete[] R;
}

template <typename T>
void sort(T arr[], int l, int r, function<bool(const T&, const T&)> cmp)
{
    if (l < r) {
        int m = l + (r - l) / 2;
        sort(arr, l, m,     cmp);
        sort(arr, m + 1, r, cmp);
        merge(arr, l, m, r,      cmp);
    }
}