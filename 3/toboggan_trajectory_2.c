#include <stdio.h>
#include <string.h>

#define INPUT_FILENAME "./input"

int count_trees(int d_x, int d_y) {
    FILE* input_file = fopen(INPUT_FILENAME, "r");

    int x = 0;
    int trees = 0;

    while(1) {

        char line[256] = { '\0' };
        fgets(line, 255, input_file);
        if (feof(input_file)) {
            return trees;
        }

        char* newline_pos = strchr(line, '\n');
        if (newline_pos != NULL && *newline_pos == '\n') {
            *newline_pos = '\0';
        }
        
        if (line[x] == '#') {
            trees++;
        }

        x = (x + d_x) % strlen(line);

        for (int i = 1; i < d_y; i++) {
            char discard[256];
            fgets(discard, 255, input_file);
        }
    }
    fclose(input_file);

    return trees;
}

int main()
{
    int a = count_trees(1, 1);
    int b = count_trees(3, 1);
    int c = count_trees(5, 1);
    int d = count_trees(7, 1);
    int e = count_trees(1, 2);

    printf("a: %d b: %d c: %d d: %d e: %d\n", a, b, c, d, e);
    printf("Tree product: %lld\n", (long long) a * b * c * d * e);

    return 0;
}