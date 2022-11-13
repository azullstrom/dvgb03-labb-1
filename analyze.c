// Namn: Anders Ullström 
// Mail: zefaniasmusic@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "analyze.h"
#include "algorithm.h"

//
// Private
//

// Sets the array elements in a certain order depending on case_t
static void set_array(int *arr, case_t c, int size) {
    switch(c) {
        case best_t:
            for(int i = 0; i < size; i++) 
                arr[i] = i + 1;       
            break;
        case worst_t:
            for(int i = 0; i < size; i++) 
                arr[i] = size - i;    
            break;
        case average_t:
            srand(time(0));
            for(int i = 0; i < size; i++) 
                arr[i] = rand() % size + 1;
            break;
        default:
            break;
    }
}

// Warmup processor by running a loop for n cycles
static void warmup_processor(int n) {
    int count = 0;

    for(int i = 0; i < n; i++)
        count++;
}

// Returns key for given case
static int get_key_by_case(algorithm_t a, case_t c, int *arr, int size) {
    int key_index = 0;
    srand(time(0));

    if(a == linear_search_t) { 
        switch(c) {
            case best_t:    key_index = 0;                      break;
            case worst_t:   key_index = size - 1;               break;
            case average_t: key_index = rand() % size - 1;      break;
            default:                                            return -1;
        }
    } else if(a == binary_search_t) {
        switch(c) {
            case best_t:    key_index = (size - 1) / 2;         break;
            case worst_t:   key_index = size - 1;               break;
            case average_t: key_index = rand() % size - 1;      break;
            default:                                            return -1;
        }
    } else {
        return -1;
    }

    return arr[key_index];
}

// Sets the last element (pivot) depending on which case for quick sort
static void set_pivot_by_case(algorithm_t a, case_t c, int *arr, int size) {
    if(a == quick_sort_t) {
        switch(c) {
            case best_t:    arr[0] = size / 2;                  break;
            case worst_t:   arr[0] = size;                      break;
            case average_t: arr[0] = rand() % size + 1;         break;
            default:                                            break;
        }
    }
}

// Developed for algorithms and cases with any of these O-notations (fast O:s): O(1), O(logn), O(n).
// Returns how many iterations the given algorithm will be tested. The return value is based on separate tests that has been done
// for each algorithm and case. Some algorithms and cases are ignored because these are not so fast.
static int get_iter_by_algo(algorithm_t a, case_t c) {
    int n = ITERATIONS; // Minimum iterations
    switch(a) {
        case bubble_sort_t:
        case insertion_sort_t:
            switch(c) {
                case best_t:    n = ITERATIONS * 2500;          break;
                case worst_t:                                   break;
                case average_t:                                 break;
                default:                                        break;
            }
            break;
        case linear_search_t:
            switch(c) {
                case best_t:    n = ITERATIONS * 250000;        break;
                case worst_t:   n = ITERATIONS * 2500;          break;
                case average_t: n = ITERATIONS * 2500;          break;
                default:                                        break;
            }
            break;
        case binary_search_t:
            n = ITERATIONS * 250000;
            break;
        case quick_sort_t:
            break;
        default:
            break;
    }
    return n;
}

// Runs the given algorithm with function pointer and calculates the time it takes to run it depending on array size.
// This function is specific for bubble sort, insertion sort and quick sort.
static double time_func_void(void (*func)(), int *arr, int size, algorithm_t a, case_t c) {
    struct timespec start, stop;
    double total_time = 0;
    int n = get_iter_by_algo(a, c);

    // Creating 2D temp[n][size] so func() always gets an unique copy of original array. This is so we won't change the same array
    // back and forth. The malloc will in some n cases be large, so the time to execute the function will increase for these cases.
    int (*temp)[size] = malloc(sizeof(int[n][size]));
    for(int i = 0; i < n; i++) 
        for(int j = 0; j < size; j++) 
            temp[i][j] = arr[j];

    // TEST START //
    // Running functions n times so fast algorithms will not run too fast for the clock to register.
    warmup_processor(WARMUP_CYCLES);
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i = 0; i < n; i++)
        func(temp[i], size); 
    clock_gettime(CLOCK_MONOTONIC, &stop);
    // TEST END //

    total_time = (stop.tv_sec * BILLION + stop.tv_nsec) - (start.tv_sec * BILLION + start.tv_nsec);
    total_time = total_time / BILLION / n; // Converting to seconds
    free(temp);

    return total_time;
}

// Runs the given algorithm with function pointer and calculates the time it takes to run it depending on array size.
// This function is specific for binary search and linear search.
static double time_func_bool(bool (*func)(), int *arr, int size, algorithm_t a, case_t c) {
    struct timespec start, stop;
    double total_time = 0;
    int n = get_iter_by_algo(a, c);
    int key = get_key_by_case(a, c, arr, size);

    // TEST START //
    // Running functions n times so fast algorithms will not run too fast for the clock to register.
    warmup_processor(WARMUP_CYCLES);
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i = 0; i < n; i++)
        func(arr, size, key); 
    clock_gettime(CLOCK_MONOTONIC, &stop);
    // TEST END //

    total_time = (stop.tv_sec * BILLION + stop.tv_nsec) - (start.tv_sec * BILLION + start.tv_nsec);
    total_time = total_time / BILLION / n; // Converting to seconds

    return total_time;
}

// Returns which complexity type this algorithm and case has
static complexity_t get_complexity_t(algorithm_t a, case_t c) {
    complexity_t type = null;

    switch(a) {
        case bubble_sort_t:
        case insertion_sort_t:
            switch(c) {
                case best_t:    type = on_t;        break;
                case worst_t:   type = on2_t;       break;
                case average_t: type = on2_t;       break;
                default:                            break;
            }
            break;
        case quick_sort_t:
            switch(c) {
                case best_t:    type = onlogn_t;    break;
                case worst_t:   type = on2_t;       break;
                case average_t: type = onlogn_t;    break;
                default:                            break;
            }
            break;
        case linear_search_t:
            switch(c) {
                case best_t:    type = oone_t;      break;
                case worst_t:   type = on_t;        break;
                case average_t: type = on_t;        break;
                default:                            break;
            }
            break;
        case binary_search_t:
            switch(c) {
                case best_t:    type = oone_t;      break;
                case worst_t:   type = ologn_t;     break;
                case average_t: type = ologn_t;     break;
                default:                            break;
            }
            break;
        default:
            break;
    }
    
    return type;
}

// Returns complexity number by Big-O-notation and index where i=0 is FASTER, i=1 BIG_O, i=2 SLOWER
static double get_complexity_number(complexity_t comp, int i, double total, int n) {
    double value = -1;

    switch(comp) {
        case oone_t:
            switch(i) {
                case FASTER:    value = total / log2(n);        break;
                case BIG_O:     value = total / 1;              break;
                case SLOWER:    value = total / n;              break;
                default:                                        break;
            }
            break;
        case ologn_t:
            switch(i) {
                case FASTER:    value = total / 1;              break;
                case BIG_O:     value = total / log2(n);        break;
                case SLOWER:    value = total / n;              break;
                default:                                        break;
            }
            break;
        case on_t:
            switch(i) {
                case FASTER:    value = total / log2(n);        break;
                case BIG_O:     value = total / n;              break;
                case SLOWER:    value = total / (n * log2(n));  break;
                default:                                        break;
            }
            break;
        case onlogn_t:
            switch(i) {
                case FASTER:    value = total/n;                break;
                case BIG_O:     value = total/(n*log2(n));      break;
                case SLOWER:    value = total/pow(n,2);         break;
                default: break;
            }
            break;
        case on2_t:
            switch(i) {
                case FASTER:    value = total/(n*log2(n));      break;
                case BIG_O:     value = total/pow(n,2);         break;
                case SLOWER:    value = total/pow(n,3);         break;
                default:                                        break;
            }
            break;
        case null:
            break;
        default:
            break;
    }
    
    return value;
}

// Sets buffer size and time for the given algorithm and case
static void set_buf(result_t *buf, algorithm_t a, case_t c, double total_time, int size, int i) {
    buf[i].size = size;
    buf[i].time = total_time;
    buf[i].complexity = get_complexity_t(a, c);
    buf[i].faster = get_complexity_number(buf[i].complexity, FASTER, buf[i].time, buf[i].size);
    buf[i].big_o = get_complexity_number(buf[i].complexity, BIG_O, buf[i].time, buf[i].size);
    buf[i].slower = get_complexity_number(buf[i].complexity, SLOWER, buf[i].time, buf[i].size);
}

//
// Public
//

// Restore result_t buffer
void restore_result(result_t *buf, int n) {
    for(int i = 0; i < n; i++) {
        buf[i].size = 0;
        buf[i].time = 0;
        buf[i].complexity = null;
        buf[i].faster = 0;
        buf[i].big_o = 0;
        buf[i].slower = 0;
    }
}

void benchmark(const algorithm_t a, const case_t c, result_t *buf, int n) {
    for(int i = 0; i < n; i++) {
        double total_time = 0;
        int arr_size = 0;
        arr_size = SIZE_START * pow(2, i);
        int *arr = malloc(arr_size * sizeof(int));

        if(a == bubble_sort_t || a == insertion_sort_t)
            set_array(arr, c, arr_size);
        else 
            set_array(arr, best_t, arr_size); // For quick sort, linear- and binary search so the list is consistent

        switch(a) {
            case bubble_sort_t:
                total_time = time_func_void(&bubble_sort, arr, arr_size, a, c);
                break;
            case insertion_sort_t:
                total_time = time_func_void(&insertion_sort, arr, arr_size, a, c);
                break;    
            case quick_sort_t:
                set_pivot_by_case(a, c, arr, arr_size);
                total_time = time_func_void(&quick_sort, arr, arr_size, a, c);
                break;
            case linear_search_t:
                total_time = time_func_bool(&linear_search, arr, arr_size, a, c);
                break;
            case binary_search_t:
                total_time = time_func_bool(&binary_search, arr, arr_size, a, c);
                break;
            default:
                break;            
        }
        free(arr);

        set_buf(buf, a, c, total_time, arr_size, i);
    }
}
