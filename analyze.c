#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "analyze.h"
#include "algorithm.h"

//
// Private
//
static void set_array(int *arr, case_t c, int size) {
    switch(c) {
        case best_t:
            for(int i = 0; i < size; i++) {
                arr[i] = i + 1;
            }
        break;
        case worst_t:
            for(int i = 0; i < size; i++) {
                arr[i] = size - i;
            }
        break;
        case average_t:
            srand(time(0));
            for(int i = 0; i < size; i++) {
                arr[i] = rand() % size + 1;
            }
        break;
    }
}

// Warmup processor by running a loop for n cycles
static void warmup_processor(int n) {
    int count = 0;
    for(int i = 0; i < n; i++) {
        count++;
    }
}

// Runs the given algorithm with function pointer and calculates the time it takes to run it depending on array size
static double time_func_void(void (*func)(), int *arr, int size) {
    struct timespec start, stop;
    double total_time = 0;
   
    warmup_processor(WARMUP_CYCLES);
    clock_gettime(CLOCK_MONOTONIC, &start);
    func(arr, size);
    clock_gettime(CLOCK_MONOTONIC, &stop);

    total_time = (stop.tv_sec * BILLION + stop.tv_nsec) - (start.tv_sec * BILLION + start.tv_nsec);
    return total_time;
}

// Returns key for given case
static int get_key_by_case(algorithm_t a, case_t c, int *arr, int size) {
    int key_index = 0;
    srand(time(0));
    if(a == linear_search_t) { 
        c == best_t ? key_index = 0 : c == worst_t ? key_index = size - 1 : c == average_t ? key_index = rand() % size - 1 : exit(1);
    } else if(a == binary_search_t) {
        c == best_t ? key_index = (size - 1) / 2 : c == worst_t ? key_index = size - 1 : c == average_t ? key_index = rand() % size - 1 : exit(7);
    } else {
        return -1;
    }
    return arr[key_index];
}

// Runs the given algorithm with function pointer and calculates the time it takes to run it depending on array size
static double time_func_bool(bool (*func)(), int *arr, int size, algorithm_t a, case_t c) {
    struct timespec start, stop;
    double total_time = 0;

    warmup_processor(WARMUP_CYCLES);
    clock_gettime(CLOCK_MONOTONIC, &start);
    func(arr, size, get_key_by_case(a, c, arr, size));
    clock_gettime(CLOCK_MONOTONIC, &stop);

    total_time = (stop.tv_sec * BILLION + stop.tv_nsec) - (start.tv_sec * BILLION + start.tv_nsec);
    return total_time;
}

// Returns which complexity type this algorithm and case has
static complexity_t get_complexity_t(algorithm_t a, case_t c) {
    complexity_t type = null;

    switch(a) {
        case bubble_sort_t:
        case insertion_sort_t:
            c == best_t ? type = on_t : c == worst_t ? type = on2_t : c == average_t ? type = on2_t : c == null; 
            break;
        case quick_sort_t:
            c == best_t ? type = onlogn_t : c == worst_t ? type = on2_t : c == average_t ? type = onlogn_t : c == null; 
            break;
        case linear_search_t:
            c == best_t ? type = oone_t : c == worst_t ? type = on_t : c == average_t ? type = on_t : c == null; 
            break;
        case binary_search_t:
            c == best_t ? type = oone_t : c == worst_t ? type = ologn_t : c == average_t ? type = ologn_t : c == null; 
            break;
    }
    return type;
}

// Sets buffer size and time for the given algorithm and case
static void set_buf(result_t *buf, algorithm_t a, case_t c, double total_time, int size, int i) {
    double total_average = total_time / BILLION / ITERATIONS; // Converting to seconds and getting average value
    buf[i].size = size;
    buf[i].time = total_average;
    buf[i].complexity = get_complexity_t(a, c);
    buf[i].faster = get_complexity_number(buf[i].complexity, FASTER, buf[i].time, buf[i].size);
    buf[i].big_o = get_complexity_number(buf[i].complexity, BIG_O, buf[i].time, buf[i].size);
    buf[i].slower = get_complexity_number(buf[i].complexity, SLOWER, buf[i].time, buf[i].size);
}

//
// Public
//

// Returns complexity number by Big-O-notation and index where i=0 is FASTER, i=1 BIG_O, i=2 SLOWER
double get_complexity_number(complexity_t comp, int i, double total, int n) {
    double value = 0;
    switch(comp) {
        case oone_t:
            i == FASTER ? value = total/log2(n) : i == BIG_O ? value = total/1 : i == SLOWER ? value = total/n : exit(2);
            break;
        case ologn_t:
            i == FASTER ? value = total/1 : i == BIG_O ? value = total/log2(n) : i == SLOWER ? value = total/n : exit(3);
            break;
        case on_t:
            i == FASTER ? value = total/log2(n) : i == BIG_O ? value = total/n : i == SLOWER ? value = total/(n*log2(n)) : exit(4);
            break;
        case onlogn_t:
            i == FASTER ? value = total/log2(n) : i == BIG_O ? value = total/(n*log2(n)) : i == SLOWER ? value = total/pow(n,2) : exit(5);
            break;
        case on2_t:
            i == FASTER ? value = total/(n*log2(n)) : i == BIG_O ? value = total/pow(n,2) : i == SLOWER ? value = total/pow(n,3) : exit(6);
            break;
        case null:
            break;
    }
    return value;
}

// Restore result_t buffer
void restore_result(result_t *buf, int n) {
    for(int i = 0; i < n; i++) {
        buf[i].size = 0;
        buf[i].time = 0;
        buf[i].complexity = null;
    }
}

void benchmark(const algorithm_t a, const case_t c, result_t *buf, int n) {
    for(int i = 0; i < n; i++) {
        double total_time = 0;
        int arr_size = 0;
        
        for(int j = 0; j < ITERATIONS; j++) {
            arr_size = SIZE_START * pow(2, i);
            int arr[arr_size];

            if(a != linear_search_t && a != binary_search_t) {
                set_array(arr, c, arr_size);
            } else {
                set_array(arr, best_t, arr_size); // For linear and binary search so the list is consistent
            }

            switch(a) {
                case bubble_sort_t:
                    total_time += time_func_void(&bubble_sort, arr, arr_size);
                    break;
                case insertion_sort_t:
                    total_time += time_func_void(&insertion_sort, arr, arr_size);
                    break;    
                case quick_sort_t:
                    total_time += time_func_void(&quick_sort, arr, arr_size);
                    break;
                case linear_search_t:
                    total_time += time_func_bool(&linear_search, arr, arr_size, a, c);
                    break;
                case binary_search_t:
                    total_time += time_func_bool(&binary_search, arr, arr_size, a, c);
                    break;
                default:
                    break;            
            }
        }
        set_buf(buf, a, c, total_time, arr_size, i);
    }
}
