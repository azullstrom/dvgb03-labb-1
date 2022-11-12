#include "algorithm.h"
//
// Private
//
static void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static int partition(int *a, int n) {
    int pivot = a[0];
    int lower = 1;
    int upper = n - 1;

    do {
        while(a[lower] <= pivot && lower <= upper) {
            lower++;
        }
        while(a[upper] > pivot && lower <= upper) {
            upper--;
        } 

        if(lower <= upper) {
            swap(&a[upper], &a[lower]);

            upper--; 
            lower++;
        }
    } while(lower <= upper); 
    swap(&a[upper], &a[0]);

    return upper;
}
//
// Public
//
void bubble_sort(int *a, int n) {
    bool swap = false;

    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(a[i] > a[j]) {
                int temp = a[i];
                a[i] = a[j];
                a[j] = temp;
                swap = true;
            }
            if(!swap) break;
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
    if(n <= 1) {
        return;
    }
    int b = partition(a, n);

    quick_sort(a, b);
    quick_sort((a + b + 1), (n - b - 1));
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
    int low = 0, high = n - 1;
    while(low <= high) {
        int mid = (low + high) / 2;
        if (v == a[mid]) {
            return true;
        } else if(v > a[mid]) { // v is on the right side
            low = mid + 1;
        } else { // v is on the left side
            high = mid - 1;
        }  
    }
    return false;  
}
