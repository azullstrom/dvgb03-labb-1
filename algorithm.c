#include "algorithm.h"
//
// Private
//

//
// Public
//
void bubble_sort(int *a, int n) {
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(a[i] > a[j]) {
                int temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
    }
}

void insertion_sort(int *a, int n) {
    for (int i = 1; i < n; i++) {
        int temp = a[i];
        int j = i - 1;
        
        while (temp < a[j] && j >= 0) {
            a[j+1] = a[j];
            j--;
        }
        a[j+1] = temp;
    }
}

void quick_sort(int *a, int n) { 
    int pivot = a[n-1]; 
    int index_largest_el = 0; 

    for(int i = 0; i < n - 1; i++) {
        if(a[i] < pivot) {  
            int temp = a[i]; 
            a[i] = a[index_largest_el]; 
            a[index_largest_el] = temp; 
            index_largest_el++; 
        } 
    }
    // Swap pivot with index_largest
    int temp = a[index_largest_el]; 
    a[index_largest_el] = a[n-1];
    a[n-1] = temp; 

    if(index_largest_el > 1) {
        quick_sort(a, index_largest_el);
    }
    if(n - index_largest_el - 1 > 1) { 
        quick_sort(a + index_largest_el + 1, n - index_largest_el - 1); 
    }
}

bool linear_search(const int *a, int n, int v) {
    for(int i = 0; i < n; i++) {
        if(a[i] == v) {
            return true;
        }
    }
    return false;  
}

bool binary_search(const int *a, int n, int v) {
    return false;  
}
