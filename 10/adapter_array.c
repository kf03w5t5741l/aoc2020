#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILENAME "./input"
#define WALL_SOCKET_JOLTS 0
#define BUILT_IN_JOLT_OFFSET 3

void print_array(int array[], int length)
{
    printf("{ ");
    for (int i = 0; i < length; i++) {
        printf("%2d ", array[i]);
    }
    printf("}\n");
}

void swap(int array[], int a, int b)
{
    array[a] += array[b];
    array[b] = array[a] - array[b];
    array[a] -= array[b];
}

int partition(int array[], int start, int end)
{
    int pivot_index = start + (end - start) / 2;

    for (int i = start; i < end; i++) {
        if (i < pivot_index) {
            if (array[i] > array[pivot_index]) {
                swap(array, i, pivot_index);
                pivot_index = i;
            }
        }
        else if (i > pivot_index) {
            if (array[i] < array[pivot_index]) {
                swap(array, i, pivot_index);
                i += pivot_index;
                pivot_index = i - pivot_index;
                i -= pivot_index;
            }
        }
    }
    return pivot_index;
}

void quicksort(int array[], int start, int end)
{
    // base case
    if (end - start < 2) {
        return;
    }

    int pivot_index = partition(array, start, end);
    quicksort(array, start, pivot_index);
    quicksort(array, pivot_index + 1, end);
}

int* read_integers(char* filename, int* length)
{
    FILE* input_file = fopen(filename, "r");

    *length = 0;
    while(!feof(input_file)) {
        char discard[256] = { '\0' };
        fgets(discard, 255, input_file);

        if (atoi(discard) == 0) {
            continue;
        }
        (*length)++;
    }
    fseek(input_file, 0, SEEK_SET);

    int* integers = calloc(*length, sizeof(int));
    int i = 0;

    while(!feof(input_file)) {
        char buffer[256] = { '\0' };
        fgets(buffer, 255, input_file);

        if (atoi(buffer) == 0) {
            continue;
        }

        integers[i] = atoi(buffer);
        i++;
    }

    fclose(input_file);

    return integers;
}

unsigned long long valid_path(int array[], int start, int end)
{
    if (start + 2 >= end) {
        return 1;
    }

    unsigned long long paths = 0;

    int j = 1;
    while (array[start + j] - array[start] <= 3) {
        paths += valid_path(array, start + j, end);
        j++;
    }

    return paths;
}

int main()
{
    int number_of_integers = 0;
    int* integers = read_integers(INPUT_FILENAME, &number_of_integers);

    quicksort(integers, 0, number_of_integers);
    
    // add wall socket and built-in adapter
    int number_of_adapters = number_of_integers + 2;
    int* adapters = malloc(number_of_adapters * sizeof(int));
    memcpy(adapters + 1, integers, number_of_integers * sizeof(int));
    free(integers);
    adapters[0] = WALL_SOCKET_JOLTS;
    adapters[number_of_adapters - 1] = adapters[number_of_adapters - 2] + BUILT_IN_JOLT_OFFSET;

    print_array(adapters, number_of_adapters);

    // count adapters with a difference of one or three between them
    int diff_one = 0;
    int diff_three = 0;

    for (int i = 0; i < number_of_adapters - 1; i++) {
        if (adapters[i + 1] - adapters[i] == 1) {
            diff_one++;
        }
        else if (adapters[i + 1] - adapters[i] == 3) {
            diff_three++;
        }
    }

    printf("diff_one: %d diff_three: %d ", diff_one, diff_three);
    printf("solution1: %d ", diff_one * diff_three);

    // determine possible adapter combinations given max 3 difference
    unsigned long long combinations = 1;

    // split up adapters into block with unskippable beginnings and endings
    int block_start = 0;
    int block_end = 0;

    while (number_of_adapters - block_start > 4) {
        block_end = block_start;
        while (adapters[block_end + 1] - adapters[block_end] != 3) {
            block_end++;
        }

        // recursively count all valid paths within the block
        combinations *= valid_path(adapters, block_start, block_end + 1);        

        block_start = block_end + 1;
    }

    printf("solution2: %llu\n", combinations);

    free(adapters);

    return 0;
}