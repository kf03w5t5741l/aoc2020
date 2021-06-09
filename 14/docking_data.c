#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Instruction {
    
    long long value;
    struct Instruction* next;
};

int main()
{
    unsigned long long mask_ones = 0;
    unsigned long long mask_zeroes = 0;
    unsigned long long mem[100000] = { 0 };

    char buffer[BUFSIZ];
    while (fgets(buffer, BUFSIZ, stdin)) {
        
        if (strncmp(buffer, "mask", strlen("mask")) == 0) {
            char* token = strtok(buffer, " ");
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            printf("mask: parsing %s\n", token);

            mask_ones = 0;
            mask_zeroes = 0;

            for (int i = 0; i < strlen(token) - 1; i++) {
                if (token[i] != 'X') {
                    unsigned long long pow_2 = 1;
                    for (int j = 0; j < strlen(token) - 2 - i; j++) {
                        pow_2 *= 2;
                    }
                    printf("%d: %c = %llu\n", i, token[i], pow_2);
                    if (token[i] == '1') {
                        mask_ones = mask_ones | pow_2;
                    }
                    else if (token[i] == '0') {
                        mask_zeroes = mask_zeroes | pow_2;
                    }
                }
            }
            printf("mask_ones: %llu\tmask_zeroes: %llu\n", mask_ones, mask_zeroes);
        }
        else if (strncmp(buffer, "mem", strlen("mem")) == 0) {
            char* token = strtok(buffer, "[");
            token = strtok(NULL, "[");
            int address = atoi(token);

            token = strtok(token, " ");
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            unsigned long long value = strtol(token, NULL, 10) ;

            value = value | mask_ones;
            value = value & ~mask_zeroes;

            mem[address] = value;

            printf("mem[%d] = %llu\t", address, value);
            printf("mask_ones: %llu\tmask_zeroes: %llu\n", mask_ones, mask_zeroes);
        }
    }

    unsigned long long sum = 0;
    for (int i = 0; i < sizeof(mem) / sizeof(mem[0]); i++) {
        sum += mem[i];
    }

    printf("sum: %llu\n", sum);
    return 0;
}