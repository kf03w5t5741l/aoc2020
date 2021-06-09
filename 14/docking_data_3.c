//  This solution initially implemented a naive array, but the given
//  bitmasks resulted in virtual memory addresses that were too large
//  to serve as array indices. A hashtable solved this problem.
//
//  A binary tree proved significantly slower, presumably due to the
//  required number of traversals and the fact the input was unsorted.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIT_WIDTH 36
#define HT_SIZE 100000

struct HtItem {
    unsigned long long key;
    int value;
    struct HtItem* next;
};

int ht_hashcode(long long key)
{
    return key % HT_SIZE;
}

struct HtItem** ht_create()
{
    return calloc(HT_SIZE, sizeof(struct HtItem*));
}

void ht_put(struct HtItem* hashtable[], unsigned long long key, int value)
{
    struct HtItem* new_item = malloc(sizeof(struct HtItem));
    new_item->key = key;
    new_item->value = value;
    new_item->next = NULL;

    int hc = ht_hashcode(key);
    struct HtItem* item = hashtable[hc];

    if (item == NULL) {
        hashtable[hc] = new_item;
    }
    else {
        while (item != NULL) {
            if (item->key == new_item->key) {
                item->value = new_item->value;
                free(new_item);
                return;
            }
            else if (item->next == NULL) {
                item->next = new_item;
                return;
            }
            item = item->next;
        }
    }
}

long long ht_sum(struct HtItem* hashtable[])
{
    if (hashtable == NULL) {
        return 0;
    }

    long long sum = 0;
    for (int i = 0; i < HT_SIZE; i++) {
        struct HtItem* item = hashtable[i];
        while (item != NULL) {
            sum += item->value;
            item = item->next;
        }
    }
    return sum;
}

void ht_free(struct HtItem* hashtable[])
{
    for (int i = 0; i < HT_SIZE; i++) {
        struct HtItem* item = hashtable[i];
        while (item != NULL) {
            struct HtItem* next = item->next;
            free(item);
            item = next;
        }
    }
    free(hashtable);
}

unsigned long long pow_2(int exponent)
{
    unsigned long long result = 1;

    for (int i = 0; i < exponent; i++) {
        result *= 2;
    }

    return result;
}


unsigned int count_set_bits(long long value)
{
    unsigned int count = 0;
    while (value != 0) {
        count += value & 1;
        value = value >> 1;
    }
    return count;
}

int main()
{
    unsigned long long surplus_bits = 0;
    for (int i = 0; i < sizeof(surplus_bits) * 8 - BIT_WIDTH; i++) {
        surplus_bits += pow_2(sizeof(surplus_bits) * 8 - 1 - i);
    }
    //printf("surplus_bits = %lld\n", (signed long long) surplus_bits);

    unsigned long long mask_ones = 0;
    unsigned long long mask_zeroes = 0;
    unsigned long long mask_floating = 0;

    struct HtItem** hashtable = ht_create();


    char buffer[BUFSIZ];
    while (fgets(buffer, BUFSIZ, stdin)) {
        
        if (strncmp(buffer, "mask", strlen("mask")) == 0) {
            char* token = strtok(buffer, " ");
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            //printf("mask: parsing %s", token);

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
            //printf("mask_ones: %llu\tmask_zeroes: %llu\tmask_floating: %llu\n", mask_ones, mask_zeroes, mask_floating);
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
                //printf("mem[%llu] = %d\n", address + offset, value);
                ht_put(hashtable, address + offset, value);
            }

        }
    }

    long long sum = ht_sum(hashtable);
    ht_free(hashtable);

    printf("sum: %llu\n", sum);
    return 0;
}