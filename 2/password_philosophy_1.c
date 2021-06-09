#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILENAME "./input"
#define DELIMITERS "- :\n"

int check(char c, int min, int max, char* s) {
    int count = 0;
    int i = 0;

    while (s[i] != '\0') {
        if (s[i] == c) {
            count++;
        }
        i++;
    }

    if (count >= min && count <= max) {
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
        int min = atoi(token);

        token = strtok(NULL, DELIMITERS);
        int max = atoi(token);

        token = strtok(NULL, DELIMITERS);
        char c = token[0];

        token = strtok(NULL, DELIMITERS);
        char* password = token;

        if (check(c, min, max, password)) {
            valid_passwords++;
        }
    }
    fclose(input_file);

    printf("Valid passwords: %d\n", valid_passwords);

    return 0;
}