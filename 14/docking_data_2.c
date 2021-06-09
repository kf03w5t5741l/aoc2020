// This was my initial succesful solution to part 2. Memory
// addresses are stored as a sorted linked list, to get
// around the fact that the given adress space exceeds
// what reasonably be allocated to a simple array.
//
// Naturally, the many list traversals to insert the
// appropriate new list element make this solution very
// slow (~6s on my machine).
//
// This was improved upon in docking_data_3.c, which
// implements a simple hashtable allowing for direct
// reads and writes, most of the time.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIT_WIDTH 36

struct MemoryBlock {
    unsigned long long address;
    int value;
    struct MemoryBlock* next;
};

unsigned long long pow_2(int exponent)
{
    unsigned long long result = 1;

    for (int i = 0; i < exponent; i++) {
        result *= 2;
    }

    return result;
}

unsigned int count_set_bits(long long n)
{
    unsigned int count = 0;
    while (n != 0) {
        count += n & 1;
        n = n >> 1;
    }
    return count;
}

void insert(struct MemoryBlock* memory, long long address, int value)
{
    if (memory == NULL) {
        return;
    }

    struct MemoryBlock* new_block = calloc(1, sizeof(struct MemoryBlock));
    new_block->address = address;
    new_block->value = value;

    while (memory != NULL) {
        if (memory->next == NULL) {
            memory->next = new_block;
            break;
        }
        else if (new_block->address > memory->address
            && new_block->address < memory->next->address) {
                new_block->next = memory->next;
                memory->next = new_block;
                break;
            }
        else if (new_block->address == memory->address) {
            memory->value = value;
            free(new_block);
            break;
        }

        memory = memory->next;
    }

}

void free_mem(struct MemoryBlock* memory)
{
    if (memory == NULL) {
        return;
    }

    if (memory->next != NULL) {
        free_mem(memory->next);
    }

    free(memory);
}

int main()
{
    unsigned long long surplus_bits = 0;
    for (int i = 0; i < sizeof(surplus_bits) * 8 - BIT_WIDTH; i++) {
        surplus_bits += pow_2(sizeof(surplus_bits) * 8 - 1 - i);
    }
    printf("surplus_bits = %lld\n", (signed long long) surplus_bits);

    unsigned long long mask_ones = 0;
    unsigned long long mask_zeroes = 0;
    unsigned long long mask_floating = 0;

    struct MemoryBlock* memory_start = calloc(1, sizeof(struct MemoryBlock));

    char buffer[BUFSIZ];
    while (fgets(buffer, BUFSIZ, stdin)) {
        
        if (strncmp(buffer, "mask", strlen("mask")) == 0) {
            char* token = strtok(buffer, " ");
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            printf("mask: parsing %s", token);

            mask_ones = 0;
            mask_zeroes = surplus_bits;

            for (int i = 0; i < strlen(token) - 1; i++) {
                if (token[i] != 'X') {
                    unsigned long long bit = 1;
                    bit = pow_2(strlen(token) - 2 - i);
                    //printf("%d: %c = %llu\n", i, token[i], bit);
                    if (token[i] == '1') {
                        mask_ones = mask_ones | bit;
                    }
                    else if (token[i] == '0') {
                        mask_zeroes = mask_zeroes | bit;
                    }
                }
            }
            mask_floating = ~(mask_ones | mask_zeroes);
            printf("mask_ones: %llu\tmask_zeroes: %llu\tmask_floating: %llu\n", mask_ones, mask_zeroes, mask_floating);
        }
        else if (strncmp(buffer, "mem", strlen("mem")) == 0) {
            char* token = strtok(buffer, "[");
            token = strtok(NULL, "[");
            unsigned long long address = atoi(token);

            token = strtok(token, " ");
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            int value = strtol(token, NULL, 10) ;

            address = address | mask_ones;
            address = address & ~(mask_floating);

            unsigned int set_bits = count_set_bits(mask_floating);
            unsigned long long combinations = pow_2(set_bits);

            for (long long i = 0; i < combinations; i++) {
                unsigned long long temp_mask = mask_floating;
                unsigned long long offset = 0;
                unsigned long long temp_i = i;

                while (temp_i != 0) {
                    unsigned long long lsb = pow_2(ffsll(temp_mask) - 1);
                    if (ffsll(temp_i) == 1) {
                        offset ^= lsb;
                    }
                    temp_mask ^= lsb;
                    temp_i >>= 1;
                }
                insert(memory_start, address + offset, value);
                //printf("mem[%llu] = %d\n", address + offset, value);
            }

        }
    }

    long long sum = 0;
    struct MemoryBlock* memory = memory_start;
    while (memory != NULL) {
        sum += memory->value;
        memory = memory->next;
    }
    printf("sum: %lld\n", sum);

    free_mem(memory_start);

    return 0;
}