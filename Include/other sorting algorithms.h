#include <iostream>
#include <functional>
#include <vector>
using namespace std;


// generic bubble sort 
template <typename T>
void bubbleSort(T arr[], int n, function<bool(const T&, const T&)> cmp)
{
    
    for (int i = 0; i < n - 1; i++) {

        
        for (int j = 0; j < n - i - 1; j++) {
            if (cmp(arr[j + 1], arr[j])) {

                
                T temp = arr[j]; arr[j] = arr[j + 1];
                arr[j + 1] = temp; }
        }
    }
}







//generic quick sort 

template <typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp; }
 

template <typename T, typename Compare>
int partition(vector<T>& arr, int low, int high, Compare cmp)
{
    T pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (cmp(arr[j], pivot)) {
            i++;
            swap(arr[i], arr[j]); }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1); }
 

template <typename T, typename Compare>
void quickSort(vector<T>& arr, int low, int high, Compare cmp)
{
    if (low < high) {
        int pi = partition(arr, low, high, cmp);
        quickSort(arr, low, pi - 1, cmp);
        quickSort(arr, pi + 1, high, cmp); }
}
 
