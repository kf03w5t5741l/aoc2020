#include <stdio.h>
#include <string.h>

#define INPUT_FILENAME "./input"

int main()
{
    FILE* input_file = fopen(INPUT_FILENAME, "r");

    int x = 0;
    int trees = 0;

    while(!feof(input_file)) {
        char line[256] = { '\0' };
        fgets(line, 255, input_file);
        char* newline_pos = strchr(line, '\n');
        if (newline_pos != NULL) {
            *newline_pos = '\0';
        }
        
        if (line[x] == '#') {
            trees++;
        }

        x = (x + 3) % strlen(line);
    }
    fclose(input_file);

    printf("Trees: %d\n", trees);

    return 0;
}