#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define INPUT_FILENAME "./input"
#define WIDTH 25

int main() {
    FILE* input_file = fopen(INPUT_FILENAME, "r");

    int size = 0;

    while(!feof(input_file)) {
        char discard[256] = { '\0' };
        fgets(discard, 255, input_file);

        if (!isdigit(discard[0])) {
            continue;
        }

        size++;
    }
    unsigned long long numbers[size];

    for (int i = 0; i < sizeof(numbers)/sizeof(numbers[0]); i++) {
        numbers[i] = 0;
    }

    fseek(input_file, 0, SEEK_SET);
    int count = 0;

    while(!feof(input_file)) {
        char buffer[256] = { '\0' };
        fgets(buffer, 255, input_file);

        if (!isdigit(buffer[0])) {
            continue;
        }

        numbers[count] = strtoull(buffer, NULL, 10);
        count++;
    }
    fclose(input_file);

    for (int i = WIDTH; i < sizeof(numbers) / sizeof(numbers[0]); i++) {
        int valid = 0;
        for (int j = i - WIDTH; j < i - 1; j++) {
            for (int k = j + 1; k < i; k++) {
                if (numbers[j] + numbers[k] == numbers[i]) {
                    valid += 1;
                }
            } 
        }

        if (!valid) {
            printf("%d: %llu is invalid\n", i, numbers[i]);
            break;
        }
    }

    return 0;
}