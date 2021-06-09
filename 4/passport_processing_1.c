#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILENAME "./input"

struct Passport {
    int* byr;
    int* iyr;
    int* eyr;
    char* hgt;
    int* hcl;
    char* ecl;
    int* pid;
    int* cid;
    struct Passport* next;
};

int main(int argc, char* argv[])
{
    FILE* input_file = fopen(INPUT_FILENAME, "r");

    struct Passport* new_passport = calloc(1, sizeof(struct Passport));
    struct Passport* passport_list = new_passport;

    int passport_counter = 0;

    while (!feof(input_file)) {
        char line[256] = { '\0' };
        fgets(line, 255, input_file);

        if (line[0] == '\n') {
            passport_counter++;
            new_passport->next = calloc(1, sizeof(struct Passport));
            new_passport = new_passport->next;
            continue;
        }
        
        char* token = strtok(line, " ");

        while (token != NULL) {
            if (strncmp(token, "byr", 3) == 0) {
                new_passport->byr = malloc(sizeof(int));
                *new_passport->byr = atoi(token + 4);
            }
            else if (strncmp(token, "iyr", 3) == 0) {
                new_passport->iyr = malloc(sizeof(int));
                *new_passport->iyr = atoi(token + 4);
            }
            else if (strncmp(token, "eyr", 3) == 0) {
                new_passport->eyr = malloc(sizeof(int));
                *new_passport->eyr = atoi(token + 4);
            }
            else if (strncmp(token, "hgt", 3) == 0) {
                new_passport->hgt = malloc(strlen(token + 4));
                strncpy(new_passport->hgt, token + 4, strlen(token + 4));
            }
            else if (strncmp(token, "hcl", 3) == 0) {
                new_passport->hcl = malloc(sizeof(int));
                char** end = NULL;
                *new_passport->hcl = strtol(token + 4, end, 16);
            }
            else if (strncmp(token, "ecl", 3) == 0) {
                new_passport->ecl = malloc(strlen(token + 4));
                strncpy(new_passport->ecl, token + 4, strlen(token + 4));
            }
            else if (strncmp(token, "pid", 3) == 0) {
                new_passport->pid = malloc(sizeof(int));
                *new_passport->pid = atoi(token + 4);
            }
            else if (strncmp(token, "cid", 3) == 0) {
                new_passport->cid = malloc(sizeof(int));
                *new_passport->cid = atoi(token + 4);
            }

            token = strtok(NULL, " ");
        }
    }
    fclose(input_file);
    printf("Passports read: %d\n", passport_counter);

    int valid_passports = 0;
    struct Passport* passport_to_check = passport_list;

    while (passport_to_check != NULL) {
        if (passport_to_check->byr != NULL
            && passport_to_check->iyr != NULL
            && passport_to_check->eyr != NULL
            && passport_to_check->hgt != NULL
            && passport_to_check->hcl != NULL
            && passport_to_check->ecl != NULL
            && passport_to_check->pid != NULL
            ) {
                valid_passports++;
            }
        passport_to_check = passport_to_check->next;
    }

    printf("Valid passports: %d\n", valid_passports);

    while (passport_list->next != NULL) {
        if (passport_list->byr != NULL)
            free(passport_list->byr);
        if (passport_list->iyr!= NULL)
            free(passport_list->iyr);
        if (passport_list->eyr != NULL)
            free(passport_list->eyr);
        if (passport_list->hgt != NULL)
            free(passport_list->hgt);
        if (passport_list->hcl != NULL)
            free(passport_list->hcl);
        if (passport_list->ecl != NULL)
            free(passport_list->ecl);
        if (passport_list->pid != NULL)
            free(passport_list->pid);
        if (passport_list->cid != NULL)
            free(passport_list->cid);

        struct Passport* free_passport = passport_list;
        passport_list = passport_list->next;
        free(free_passport);
    }
}