#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INPUT_FILENAME "./input"

enum HEIGHT_UNIT {
    CM,
    IN
};

enum EYE_COLOUR {
    AMB,
    BLU,
    BRN,
    GRY,
    GRN,
    HZL,
    OTH
};

char* eye_colours[] = {
    "amb",
    "blu",
    "brn",
    "gry",
    "grn",
    "hzl",
    "oth"
};

struct Passport {
    int* byr;
    int* iyr;
    int* eyr;
    int* hgt;
    enum HEIGHT_UNIT* hgt_unit;
    int* hcl;
    enum EYE_COLOUR* ecl;
    int* pid;
    int* cid;
    struct Passport* next;
};

struct Passport* read_passports(char* filename)
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
                int invalid_chars = 0;
                for (int i = 0; i < 4; i++) {
                    if (!isdigit((token + 4)[i])) {
                        invalid_chars++;
                        break;
                    }
                }
                if (invalid_chars > 0) {
                    printf("%s\n", token + 4);
                    token = strtok(NULL, " ");
                    continue;
                }

                new_passport->byr = calloc(1, sizeof(int));
                *new_passport->byr = atoi(token + 4);
            }
            else if (strncmp(token, "iyr", 3) == 0) {
                int invalid_chars = 0;
                for (int i = 0; i < 4; i++) {
                    if (!isdigit((token + 4)[i])) {
                        invalid_chars++;
                        break;
                    }
                }
                if (invalid_chars > 0) {
                    printf("%s\n", token + 4);
                    token = strtok(NULL, " ");
                    continue;
                }

                new_passport->iyr = calloc(1, sizeof(int));
                *new_passport->iyr = atoi(token + 4);
            }
            else if (strncmp(token, "eyr", 3) == 0) {
                int invalid_chars = 0;
                for (int i = 0; i < 4; i++) {
                    if (!isdigit((token + 4)[i])) {
                        invalid_chars++;
                        break;
                    }
                }
                if (invalid_chars > 0) {
                    printf("%s\n", token + 4);
                    token = strtok(NULL, " ");
                    continue;
                }

                new_passport->eyr = calloc(1, sizeof(int));
                *new_passport->eyr = atoi(token + 4);
            }
            else if (strncmp(token, "hgt", 3) == 0) {
                char* text_start = NULL;
                if (strchr(token + 4, 'c') - strchr(token + 4, 'm') == -1) {
                    text_start = strchr(token + 4, 'c');
                    new_passport->hgt_unit = calloc(1, sizeof(enum HEIGHT_UNIT));
                    *new_passport->hgt_unit = CM;
                }
                else if (strchr(token + 4, 'i') - strchr(token + 4, 'n') == -1) {
                    text_start = strchr(token + 4, 'c');
                    new_passport->hgt_unit = calloc(1, sizeof(enum HEIGHT_UNIT));
                    *new_passport->hgt_unit = IN;
                }
                else {
                    token = strtok(NULL, " ");
                    continue;
                }

                int invalid_chars = 0;
                for (char* i = token + 4; i < text_start; i++) {
                    if (!isdigit(*i)) {
                        invalid_chars++;
                        break;
                    }
                }
                if (invalid_chars > 0) {
                    token = strtok(NULL, " ");
                    continue;
                }

                new_passport->hgt = calloc(1, sizeof(int));
                *new_passport->hgt = atoi(token + 4);
            }
            else if (strncmp(token, "hcl", 3) == 0) {
                if (token[4] != '#') {
                    token = strtok(NULL, " ");
                    continue;
                }
               
                int bad_chars = 0;
                for (int i = 0; i < 6; i++) {
                    if (!isxdigit((token + 5)[i])) {
                        bad_chars++;
                    }
                }
                if (bad_chars > 0) {
                    token = strtok(NULL, " ");
                    continue;
                }
                
                new_passport->hcl = calloc(1, sizeof(int));
                *new_passport->hcl = strtol(token + 5, NULL, 16);
            }
            else if (strncmp(token, "ecl", 3) == 0) {
                int size = sizeof(eye_colours)/sizeof(eye_colours[0]);
                for (int i = 0; i < size; i++) {
                    if (strncmp(token + 4, eye_colours[i], 3) == 0
                        && !isalpha((token + 4)[3])) {
                        new_passport->ecl = calloc(1, sizeof(eye_colours[0]));
                        *new_passport->ecl = i;
                        break;
                    }
                }
            }
            else if (strncmp(token, "pid", 3) == 0) {
                int invalid_chars = 0;
                for (int i = 0; i < 9; i++) {
                    if (!isdigit((token + 4)[i])) {
                        invalid_chars++;
                    }
                }
                if (isdigit((token + 4)[9])) {
                    invalid_chars++;
                }
                if (invalid_chars > 0) {
                    token = strtok(NULL, " ");
                    continue;
                }

                new_passport->pid = calloc(1, sizeof(int));
                *new_passport->pid = atoi(token + 4);
            }
            else if (strncmp(token, "cid", 3) == 0) {
                new_passport->cid = calloc(1, sizeof(int));
                *new_passport->cid = atoi(token + 4);
            }

            token = strtok(NULL, " ");
        }
    }
    fclose(input_file);
    printf("Passports read: %d\n", passport_counter);
    return passport_list;
}

int check_passport(struct Passport* passport) {
    if (passport->byr != NULL
        && *passport->byr >= 1920
        && *passport->byr <= 2002

        && passport->iyr != NULL
        && *passport->iyr >= 2010
        && *passport->iyr <= 2020

        && passport->eyr != NULL
        && *passport->eyr >= 2020
        && *passport->eyr <= 2030

        && passport->hgt != NULL
        && passport->hgt_unit != NULL
        && ((*passport->hgt_unit == CM
                && *passport->hgt >= 150
                && *passport->hgt <= 193)
            || (*passport->hgt_unit == IN)
                && *passport->hgt >= 59
                && *passport->hgt <= 76)

        && passport->hcl != NULL

        && passport->ecl != NULL

        && passport->pid != NULL
        ) {
            return 1;
        }
            
    return 0;
}

void free_passports(struct Passport* passport_list)
{
    while (passport_list->next != NULL) {
        if (passport_list->byr != NULL)
            free(passport_list->byr);
        if (passport_list->iyr!= NULL)
            free(passport_list->iyr);
        if (passport_list->eyr != NULL)
            free(passport_list->eyr);
        if (passport_list->hgt != NULL)
            free(passport_list->hgt);
        if (passport_list->hgt_unit != NULL)
            free(passport_list->hgt_unit);
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

int main(int argc, char* argv[])
{
    struct Passport* passport_list = read_passports(INPUT_FILENAME);

    int valid_passports = 0;
    struct Passport* current_passport = passport_list;

    while (current_passport != NULL) {
        if (check_passport(current_passport)) {
            valid_passports++;
        }
        current_passport = current_passport->next;
    }
    printf("Valid passports: %d\n", valid_passports);

    free_passports(passport_list);

    return 0;
}