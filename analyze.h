#ifndef ANALYZE_H
#define ANALYZE_H

#define SIZE_START 512
#define ITERATIONS 4
#define BILLION 1000000000
#define WARMUP_CYCLES 1000000
#define FASTER 0
#define BIG_O 1
#define SLOWER 2

// complexity_t defines different time complexities
typedef enum {
    null,
    oone_t,
    ologn_t,
    on_t,
    onlogn_t,
    on2_t,
} complexity_t;

// algorithm_t defines different algorithms
typedef enum {
    bubble_sort_t,
    insertion_sort_t,
    quick_sort_t,
    linear_search_t,
    binary_search_t
} algorithm_t;

// case_t defines different cases
typedef enum {
    best_t,
    worst_t,
    average_t,
} case_t;

// result_t defines a timed measurement for a given array size
typedef struct {
    int size;
    double time;
    double faster;
    double big_o;
    double slower;
    complexity_t complexity;
} result_t;

// Restore result_t buffer
void restore_result(result_t *buf, int n);

// benchmark benchmarks an algorithm a for a specific case c, writing n results
// to the result buffer buf
void benchmark(const algorithm_t a, const case_t c, result_t *buf, int n);

#endif
