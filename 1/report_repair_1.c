#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILENAME "./input"
#define TARGET_SUM 2020

int check(int a, int b) {
    if (a + b == TARGET_SUM) {
        return 1;
    }
    else {
        return 0;
    }
}

int* get_integers(char* filename, int* array_size)
{
    FILE* input_file = fopen(filename, "r");

    *array_size = 0;

    while(!feof(input_file)) {
        int unused;
        fscanf(input_file, "%d", &unused);
        (*array_size)++;
    }
    fseek(input_file, 0, SEEK_SET);

    int* integers = malloc(sizeof(int) * *array_size);
    if (integers == NULL) {
        return NULL;
    }

    for (int i = 0; i < *array_size; i++) {
        fscanf(input_file, "%d", &integers[i]);
    }

    int input_file_closed = fclose(input_file);

    return integers;
}

int search(int integers[], int integers_size, int target) {
    for (int i = 0; i < integers_size; i++) {
        for (int j = i + 1; j < integers_size; j++) {
            if (check(integers[i], integers[j])) {
                return integers[i] * integers[j];
            };
        }
    }

    return -1;
}

int main(int argc, char* argv[]) {
    int integers_size;
    int* integers = get_integers(INPUT_FILENAME, &integers_size);
    
    int target = search(integers, integers_size, TARGET_SUM);
    printf("%d\n", target);

    free(integers);

    return 0;
}