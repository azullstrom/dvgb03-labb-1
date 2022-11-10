#include "ui.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "analyze.h"
#include "io.h"

//
// Private
//
static void ui_invalid_input() { printf("info> bad input\n"); }

static void ui_exit() { printf("info> bye\n"); }

static char ui_get_choice() {
    char buf[3];

    printf("input> ");
    return read_line(buf, 3) ? buf[0] : 0;
}

static void ui_line(char c, int n) {
    while (n-- > 0) {
        putchar(c);
    }
    putchar('\n');
}

static void ui_menu_options(const char *options[], int num_options) {
    int i;

    for (i = 0; i < num_options; i++) {
        printf("    %c) %s\n", 'a' + i, options[i]);
    }
}

static void ui_menu() {
    const char *options[] = {
        "Menu",
        "Exit\n",
        "Bubble sort best case",
        "Bubble sort worst case",
        "Bubble sort average case\n",
        "Insertion sort best case",
        "Insertion sort worst case",
        "Insertion sort average case\n",
        "Quick sort sort best case",
        "Quick sort worst case",
        "Quick sort average case\n",
        "Linear search best case",
        "Linear search worst case",
        "Linear search average case\n",
        "Binary search best case",
        "Binary search worst case",
        "Binary search average case",
    };

    ui_line('=', MENU_WIDTH);
    ui_menu_options(options, sizeof(options) / sizeof(char *));
    ui_line('-', MENU_WIDTH);
}

// Get string depending on algorithm_t
static const char* algorithm_str[] = {
    [bubble_sort_t] = "Bubble Sort",
    [insertion_sort_t] = "Insertion Sort",
    [quick_sort_t] = "Quick Sort",
    [linear_search_t] = "Linear Sort",
    [binary_search_t] = "Binary Sort",
};

// Get string depending on case_t
static const char* case_str[] = {
    [best_t] = "Best",
    [worst_t] = "Worst",
    [average_t] = "Average",
};

// Get string depending on complexity_t
static const char* complexity_str[] = {
    [oone_t] = "Size\tTime T(s)\tT/logn\t\tT/1\t\tT/n\n",
    [ologn_t] = "Size\tTime T(s)\tT/1\t\tT/logn\t\tT/n\n",
    [on_t] = "Size\tTime T(s)\tT/logn\t\tT/n\t\tT/nlogn\n",
    [onlogn_t] = "Size\tTime T(s)\tT/n\t\tT/nlogn\t\tT/n^2\n",
    [on2_t] = "Size\tTime T(s)\tT/nlogn\t\tT/n^2\t\tT/n^3\n",
};

// Print result menu with size, time and time complexity
static void ui_print_result(algorithm_t a, case_t c, result_t *buf, int n) {
    ui_line('*', MENU_WIDTH * 2);
    printf("\t\t\t%s: %s\n", algorithm_str[a], case_str[c]);
    ui_line('~', MENU_WIDTH * 2);
    printf("%s", complexity_str[buf[0].complexity]);
    ui_line('~', MENU_WIDTH * 2);

    for(int i = 0; i < n; i++) {
        printf("%d\t%.10f\t%e\t%e\t%e\n", buf[i].size, buf[i].time, buf[i].faster, buf[i].big_o, buf[i].slower);
    }
    ui_line('*', MENU_WIDTH * 2);
}

//
// Public
//
void ui_run() {
    bool running, show_menu;
    result_t result[RESULT_ROWS];

    show_menu = true;
    running = true;
    while (running) {
        if (show_menu) {
            show_menu = false;
            ui_menu();
        }
        switch (ui_get_choice()) {
            // House keeping
            case 'a':
                show_menu = true;
                break;
            case 'b':
                running = false;
                break;
            // Bubble sort
            case 'c':
                benchmark(bubble_sort_t, best_t, result, RESULT_ROWS);
                ui_print_result(bubble_sort_t, best_t, result, RESULT_ROWS);
                break;
            case 'd':
                benchmark(bubble_sort_t, worst_t, result, RESULT_ROWS);
                ui_print_result(bubble_sort_t, worst_t, result, RESULT_ROWS);
                break;
            case 'e':
                benchmark(bubble_sort_t, average_t, result, RESULT_ROWS);
                ui_print_result(bubble_sort_t, average_t, result, RESULT_ROWS);
                break;
            // Insertion sort
            case 'f':
                benchmark(insertion_sort_t, best_t, result, RESULT_ROWS);
                ui_print_result(insertion_sort_t, best_t, result, RESULT_ROWS);
                break;
            case 'g':
                benchmark(insertion_sort_t, worst_t, result, RESULT_ROWS);
                ui_print_result(insertion_sort_t, worst_t, result, RESULT_ROWS);
                break;
            case 'h':
                benchmark(insertion_sort_t, average_t, result, RESULT_ROWS);
                ui_print_result(insertion_sort_t, average_t, result, RESULT_ROWS);
                break;
            // Quick sort
            case 'i':
                benchmark(quick_sort_t, best_t, result, RESULT_ROWS);
                ui_print_result(quick_sort_t, best_t, result, RESULT_ROWS);
                break;
            case 'j':
                benchmark(quick_sort_t, worst_t, result, RESULT_ROWS);
                ui_print_result(quick_sort_t, worst_t, result, RESULT_ROWS);
                break;
            case 'k':
                benchmark(quick_sort_t, average_t, result, RESULT_ROWS);
                ui_print_result(quick_sort_t, average_t, result, RESULT_ROWS);
                break;
            // Linear search
            case 'l':
                benchmark(linear_search_t, best_t, result, RESULT_ROWS);
                ui_print_result(linear_search_t, best_t, result, RESULT_ROWS);
                break;
            case 'm':
                benchmark(linear_search_t, worst_t, result, RESULT_ROWS);
                ui_print_result(linear_search_t, worst_t, result, RESULT_ROWS);
                break;
            case 'n':
                benchmark(linear_search_t, average_t, result, RESULT_ROWS);
                ui_print_result(linear_search_t, average_t, result, RESULT_ROWS);
                break;
            // Binary search
            case 'o':
                benchmark(binary_search_t, best_t, result, RESULT_ROWS);
                ui_print_result(binary_search_t, best_t, result, RESULT_ROWS);
                break;
            case 'p':
                benchmark(binary_search_t, worst_t, result, RESULT_ROWS);
                ui_print_result(binary_search_t, worst_t, result, RESULT_ROWS);
                break;
            case 'q':
                benchmark(binary_search_t, average_t, result, RESULT_ROWS);
                ui_print_result(binary_search_t, average_t, result, RESULT_ROWS);
                break;
            // Invalid input
            default:
                show_menu = false;
                ui_invalid_input();
                break;
        }
        restore_result(result, RESULT_ROWS);
    }
    ui_exit();
}
