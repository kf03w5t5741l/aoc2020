#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TARGET_STR "departure"

struct TicketField {
    char* name;
    int range1_low;
    int range1_high;
    int range2_low;
    int range2_high;
    
    struct TicketField* prev;
};

struct TicketField* parse_field(char* field_str)
{
    struct TicketField* field = malloc(sizeof(struct TicketField));
    field->name = calloc(255, sizeof(char));
    field->prev = NULL;

    sscanf(
        field_str,
        "%254[^:]: %u-%u or %u-%u",
        field->name,
        &field->range1_low,
        &field->range1_high,
        &field->range2_low,
        &field->range2_high
    );

    return field;
}

void free_fields(struct TicketField* field)
{
    if (field == NULL) {
        return;
    }

    if (field->prev != NULL) {
        free_fields(field->prev);
    } 

    if (field->name != NULL) {
        free(field->name);
    }

    free(field);
}

int parse_ticket(
    int field_count,
    int ticket[field_count],
    char* csv_ticket)
{
    char* csv = strtok(csv_ticket, ",");

    for (int i = 0; i < field_count; i++) {
        ticket[i] = atoi(csv);
        csv = strtok(NULL, ",");
    }

    return 1;
}

int check_field(struct TicketField* field, int value)
{
    if (field == NULL) {
        return 0;
    }

    if (value >= field->range1_low && value <= field->range1_high) {
        return 1;
    }
    else if (value >= field->range2_low && value <= field->range2_high) {
        return 1;
    }
    else {
        return 0;
    }
}

int check_entry(
    int field_count,
    struct TicketField* fields[field_count],
    int value)
{
    for (int i = 0; i < field_count; i++) {
        if (check_field(fields[i], value)) {
            return 1;
        }
    }
    return 0;
}

int check_ticket(
    int field_count,
    struct TicketField* fields[field_count],
    int ticket[field_count])
{
    for (int i = 0; i < field_count; i++) {
        if (!check_entry(field_count, fields, ticket[i])) {
            return 0;
        }
    }
    return 1;
}

int sum_invalid_entries(
    int field_count,
    struct TicketField* fields[field_count],
    int ticket[field_count])
{
    int sum = 0;
    for (int i = 0; i < field_count; i++) {
        if (!check_entry(field_count, fields, ticket[i])) {
            sum += ticket[i];
        }
    }
    return sum;
}

int count_elements(int size, struct TicketField* fields[size])
{
    int elements = 0;
    for (int i = 0; i < size; i++) {
        if (fields[i] != NULL) {
            elements++;
        } 
    }
    return elements;
}

int count_all_elements(int size, struct TicketField* fields[size][size])
{
    int all_elements = 0;
    for (int i = 0; i < size; i++) {
        all_elements += count_elements(size, fields[i]);
    }
    return all_elements;
}

int main()
{
    char buffer[BUFSIZ] = { '\0' };
    fgets(buffer, BUFSIZ, stdin);

    // read the fields
    struct TicketField* field_list = NULL;
    int field_count = 0;

    while (buffer[0] != '\n') {
        struct TicketField* field = parse_field(buffer);
        field->prev = field_list;
        field_list = field;
        field_count++;

        fgets(buffer, BUFSIZ, stdin);
    }

    struct TicketField* fields[field_count];
    struct TicketField* field_iterator = field_list;
    for (int i = field_count - 1; i >= 0; i--) {
        fields[i] = field_iterator;
        field_iterator = field_iterator->prev;
    }

    // skip irrelevant input
    while (!isdigit(buffer[0])) {
        fgets(buffer, BUFSIZ, stdin);
    }

    // read own ticket
    int my_ticket[field_count];
    parse_ticket(field_count, my_ticket, buffer);

    // check given nearby tickets
    int sum_all_invalid_entries = 0;
    struct TicketField* positions[field_count][field_count];
    for (int i = 0; i < field_count; i++) {
        for (int j = 0; j < field_count; j++) {
            positions[i][j] = fields[j];
        }
    }

    struct TicketNode* ticket_list = NULL;
    while(fgets(buffer, BUFSIZ, stdin)) {
        if (!isdigit(buffer[0])) {
            continue;
        }

        int ticket[field_count];
        parse_ticket(field_count, ticket, buffer);

        // if the ticket is valid, store the fields for which
        // each column is a valid solution
        if (check_ticket(field_count, fields, ticket)) {

            for (int i = 0; i < field_count; i++) {
                for (int j = 0; j < field_count; j++) {

                    if (!check_field(fields[j], ticket[i])) {
                        positions[i][j] = NULL;
                    }
                }
            }
        }
        // else: for part1, sum the values of each invalid entry
        else {
            sum_all_invalid_entries += sum_invalid_entries(field_count, fields, ticket);
        }
    }
    printf("part1: %d\n", sum_all_invalid_entries);

    // find the position of each field by working back
    // from the position that has only one possible field
    // and eliminating the field from all other positions,
    // then repeat that step for the next position that
    // has only one field left, etc. lacks caching of
    // previously solved positions.
    while (count_all_elements(field_count, positions) > field_count) {
        for (int i = 0; i < field_count; i++) {
            if (count_elements(field_count, positions[i]) == 1) {
                int valid_field = 0;

                for (int j = 0; j < field_count; j++) {
                    if (positions[i][j] != NULL) {
                        valid_field = j;
                        break;
                    }
                }

                for (int j = 0; j < field_count; j++) {
                    if (j == i) {
                        continue;
                    }
                    positions[j][valid_field] = 0;
                }
            }
        }
    }

    // for part2, multiply the values of my_ticket for fields
    // that start with the string "departure"
    unsigned long long departure_multiples = 1;
    for (int i = 0; i < field_count; i++) {
        for (int j = 0; j < field_count; j++) {
            if (positions[i][j] != NULL && strncmp(positions[i][j]->name, TARGET_STR, strlen(TARGET_STR)) == 0) {
                departure_multiples *= my_ticket[i];
            }
        }
    }
    printf("part2: %llu\n", departure_multiples);

    free_fields(field_list);

    return 0;
}