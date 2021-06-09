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

            for (int j = 0; j < i - 1; j++) {
                long long sum = numbers[j];
                int k = j;

                while (sum < numbers[i] && k < i - 2) {
                    k++;
                    sum+= numbers[k];
                }
                if (sum == numbers[i]) {
                    long long hi = 0;
                    long long lo = numbers[i];
                    
                    for (int l = j; l <= k; l++) {
                        if (numbers[l] > hi) {
                            hi = numbers[l];
                        }
                        if (numbers[l] < lo) {
                            lo = numbers[l];
                        }
                    }
                    printf("Found range: j + k = [%d] + [%d] => ", j, k);
                    printf("hi + lo = %llu + %llu = %llu\n", hi, lo, hi + lo);
                }
            }
        }
    }

    return 0;
}