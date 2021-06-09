#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILENAME "./input"
#define DELIMITERS "- :\n"

int check(char c, int first_position, int second_position, char* s) {
    int hits = 0;

    if (s[first_position - 1] == c) {
        hits++;
    }

    if (s[second_position - 1] == c) {
        hits++;
    }

    printf("%c %d %d %s %d\n", c, first_position, second_position, s, hits);
    if (hits == 1) {
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    int valid_passwords = 0;

    FILE* input_file = fopen(INPUT_FILENAME, "r");

    while (!feof(input_file)) {
        char line[256] = { '\0' };
        fgets(line, 255, input_file);
        
        char* token = strtok(line, DELIMITERS);
        int first_position = atoi(token);

        token = strtok(NULL, DELIMITERS);
        int second_position = atoi(token);

        token = strtok(NULL, DELIMITERS);
        char c = token[0];

        token = strtok(NULL, DELIMITERS);
        char* password = token;

        if (check(c, first_position, second_position, password)) {
            valid_passwords++;
        }
    }
    fclose(input_file);

    printf("Valid passwords: %d\n", valid_passwords);

    return 0;
}