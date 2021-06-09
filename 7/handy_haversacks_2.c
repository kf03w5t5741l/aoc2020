#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILENAME "./input"
#define TABLE_SIZE 10000

struct Rule {
    struct Bag* bag;
    int amount;
    struct Rule* next;
};

struct Bag {
    char* quality;
    char* colour;
    struct Rule* rules;
};

struct HashtableItem {
    struct Bag* bag;
};

int ht_hashcode(struct Bag* bag)
{
    int char_sum = 0;
    for (int i = 0; i < strlen(bag->quality); i++) {
        char_sum += bag->quality[i];
    }
    for (int i = 0; i < strlen(bag->colour); i++) {
        char_sum += bag->colour[i];
    }
    
    return char_sum % TABLE_SIZE;
}

int ht_equals(struct Bag* a, struct Bag* b)
{
    if (a == NULL || a->quality == NULL || a->colour == NULL
        || b == NULL || b->quality == NULL || b->colour == NULL) {
            return 0;
    }

    if (strncmp(a->quality, b->quality, strlen(a->quality) - 1) == 0
        && strncmp(a->colour, b->colour, strlen(a->colour) - 1) == 0) {
            return 1;
    }
    else {
        return 0;
    }
}

struct HashtableItem* ht_get(struct HashtableItem* hashtable[TABLE_SIZE], struct Bag* bag) 
{
    int index = ht_hashcode(bag);

    while (hashtable[index] != NULL) {
        if (ht_equals(hashtable[index]->bag, bag)) {
            return hashtable[index];
        } else {
            index++;
            index %= TABLE_SIZE;
        }
    }
    return NULL;
}

void ht_put(struct HashtableItem* hashtable[TABLE_SIZE], struct Bag* bag)
{
    struct HashtableItem* item = calloc(1, sizeof(struct HashtableItem));
    item->bag = bag;

    int index = ht_hashcode(bag);
    while (hashtable[index] != NULL) {
        index++;
        index % TABLE_SIZE;
    }
    hashtable[index] = item;
}

void free_rules(struct Rule* rules)
{
    while (rules != NULL) {
        free_rules(rules->next);
        free(rules);
    }
}

void free_bag(struct Bag* bag)
{
    if (bag == NULL) {
        return;
    }
    if (bag->quality != NULL) {
        free(bag->quality);
    }
    if (bag->colour != NULL) {
        free(bag->colour);
    }
    if (bag->rules != NULL) {
        free_rules(bag->rules);
    }
    free(bag);
}

void ht_free(struct HashtableItem* hashtable[TABLE_SIZE])
{
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashtable[i] != NULL) {
            struct HashtableItem* item = hashtable[i];
            if (&item->bag != NULL) {
                free_bag(&item->bag);
            }
            free(item);
        }
    }
}

void check_rules_recurse(struct Bag* bag, int* result, int prev)
{
    if (bag->rules == NULL) {
        (*result) += prev;
        return;
    }

    struct Rule* rule = bag->rules;
    while (rule != NULL) {
        check_rules_recurse(rule->bag, result, prev * rule->amount);

        if (rule->next == NULL) {
            (*result) += prev;
        }
        rule = rule->next;
    }
}

int main()
{
    struct HashtableItem** bagtable = calloc(TABLE_SIZE, sizeof(struct HashtableItem));

    FILE* input_file = fopen(INPUT_FILENAME, "r");

    while (!feof(input_file)) {
        char buffer[256] = { '\0' };
        fgets(buffer, 255, input_file);

        if (buffer[0] < 'a' || buffer[0] > 'z') {
            continue;
        }

        struct Bag* bag = calloc(1, sizeof(bag));
        bag->quality = strdup(strtok(buffer, " "));
        bag->colour = strdup(strtok(NULL, " "));

        if (ht_get(bagtable, bag) == NULL) {
            ht_put(bagtable, bag);
        }
        else {
            bag = ht_get(bagtable, bag)->bag;
        }

        strtok(NULL, " ");
        strtok(NULL, " ");
        char* amount_string = strtok(NULL, " ");

        while (amount_string != NULL) {
            if (amount_string[0] == 'n') {
                break;
            }
            struct Rule* rule = calloc(1, sizeof(struct Rule));
            rule->amount = atoi(amount_string);

            struct Bag* rule_bag = calloc(1, sizeof(struct Bag));
            rule_bag->quality = strdup(strtok(NULL, " " ));
            rule_bag->colour = strdup(strtok(NULL, " " ));

            if (ht_get(bagtable, rule_bag) == NULL) {
                ht_put(bagtable, rule_bag);
            }
            rule_bag = ht_get(bagtable, rule_bag)->bag;
            rule->bag = rule_bag;
            
            rule->next = bag->rules;
            bag->rules = rule;

            strtok(NULL, " ");
            amount_string = strtok(NULL, " ");
        }
    }

    fclose(input_file);

    struct Bag* search = calloc(1, sizeof(struct Bag));
    search->quality = "shiny";
    search->colour = "gold";
    search = ht_get(bagtable, search)->bag;

    int required_bags = 0;
    int prev = 1;
    check_rules_recurse(search, &required_bags, prev);
    required_bags--;
    printf("Required bags: %d\n", required_bags);

    //ht_free(&bagtable); -> grab yer wellies
    return 0;
}