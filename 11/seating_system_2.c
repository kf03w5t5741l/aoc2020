#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILENAME "./input"

struct Seatmap {
    int rows;
    int columns;
    char** seats;
};

void print_seatmap(struct Seatmap* seatmap)
{
    printf("rows: %d columns: %d\n", seatmap->rows, seatmap->columns);
    for (int y = 0; y < seatmap->rows; y++) {
        for (int x = 0; x < seatmap->columns; x++) {
            printf("%c", seatmap->seats[y][x]);
        }
        printf("\n");
    }
}

struct Seatmap* read_seatmap(char* filename)
{
    struct Seatmap* seatmap = calloc(1, sizeof(struct Seatmap));

    FILE* input_file = fopen(filename, "r");

    while (1) {
        char discard[BUFSIZ];
        char* line = fgets(discard, BUFSIZ, input_file);

        if (line == NULL) {
            break;
        }

        if (seatmap->columns == 0) {
            seatmap->columns = strlen(discard) - 1;
        }

        seatmap->rows++;
    }
    fseek(input_file, 0, SEEK_SET);

    seatmap->seats = malloc(seatmap->rows * sizeof(char *));
    for (int y = 0; y < seatmap->rows; y++) {
        seatmap->seats[y] = malloc(seatmap->columns * sizeof(char));
    }

    int row;
    int column;

    while (1) {
        char seat = fgetc(input_file);

        if (seat == EOF) {
            break;
        }

        if (seat == '\n') {
            column = 0;
            row++;
            continue;
        }

        seatmap->seats[row][column] = seat;
        column++;
    }

    fclose(input_file);

    return seatmap;
}

struct Seatmap* next_seatmap(struct Seatmap* seatmap)
{
    struct Seatmap* new_seatmap = calloc(1, sizeof(struct Seatmap));
    new_seatmap->rows = seatmap->rows;
    new_seatmap->columns = seatmap->columns;
    new_seatmap->seats = malloc(seatmap->rows * sizeof(char *));

    for (int i = 0; i < new_seatmap->rows; i++) {
        new_seatmap->seats[i] = malloc(new_seatmap->columns * sizeof(char));
    }

    for (int y = 0; y < seatmap->rows; y++) {
        for (int x = 0; x < seatmap->columns; x++) {
            int occupied = 0;

            for(int d_y = -1; d_y <= 1; d_y++) {
                for (int d_x = -1; d_x <= 1; d_x++) {
                    if (d_x == 0 && d_y == 0) {
                        continue;
                    }

                    int check_x = x + d_x;
                    int check_y = y + d_y;

                    while (check_x >= 0 && check_x < seatmap->columns
                            && check_y >= 0 && check_y < seatmap->rows) {
                        if (seatmap->seats[check_y][check_x] == '#') {
                            occupied++;
                            break;
                        }
                        else if (seatmap->seats[check_y][check_x] == 'L') {
                            break;
                        }

                        check_x += d_x;
                        check_y += d_y;
                    }
                }
            }

            if (seatmap->seats[y][x] == 'L' && occupied == 0) {
                 new_seatmap->seats[y][x] = '#';
            }
            else if (seatmap->seats[y][x] == '#' && occupied > 4) {
                new_seatmap->seats[y][x] = 'L';
            }
            else {
                new_seatmap->seats[y][x] = seatmap->seats[y][x];
            }
        }
    }

    return new_seatmap;
}

int has_changed(struct Seatmap* a, struct Seatmap* b)
{
    for (int y = 0; y < a->rows; y++) {
        if (memcmp(a->seats[y], b->seats[y], a->columns) != 0) {
            return 1;
        }
    }
    return 0;
}

int count_occupied(struct Seatmap* seatmap)
{
    int occupied = 0;
    for (int y = 0; y < seatmap->rows; y++) {
        for (int x = 0; x < seatmap->columns; x++) {
            if (seatmap->seats[y][x] == '#') {
                occupied++;
            }
        }
    }
    return occupied;
}

void free_seatmap(struct Seatmap* seatmap)
{
    if (seatmap == NULL) {
        return;
    }

    if (seatmap->seats != NULL) {
        for (int y = 0; y < seatmap->rows; y++) {
            if (seatmap->seats[y] != NULL) {
                free(seatmap->seats[y]);
            }
        }
        free(seatmap->seats);
    }
    free(seatmap);
}

int main() {
    struct Seatmap* seatmap = read_seatmap(INPUT_FILENAME);

    print_seatmap(seatmap);

    struct  Seatmap* new_seatmap = next_seatmap(seatmap);

    while (has_changed(new_seatmap, seatmap)) {
        free_seatmap(seatmap);
        seatmap = new_seatmap;
        new_seatmap = next_seatmap(seatmap);
    }
    print_seatmap(new_seatmap);
    printf("occupied: %d\n", count_occupied(new_seatmap));

    free_seatmap(seatmap);
    free_seatmap(new_seatmap);

    return 0;
}