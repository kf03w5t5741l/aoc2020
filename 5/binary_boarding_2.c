#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILENAME "./input"
#define ROWS 128
#define COLUMNS 8

int calculate_sid(int row, int column) {
    return row * COLUMNS + column;
}

char** read_bst_seats(char* filename, int* entries)
{
    *entries = 0;
    int length = 0;

    FILE* input_file = fopen(INPUT_FILENAME, "r");

    while (!feof(input_file)) {
        char discard[256] = { '\0' };
        fgets(discard, 255, input_file);
        (*entries)++;
    }
    fseek(input_file, 0, SEEK_SET);

    char** bst_seats = malloc((*entries) * sizeof(char*));

    for (int i = 0; i < *entries; i++) {
        char line[256] = { '\0' };
        fgets(line, 255, input_file);
        bst_seats[i] = calloc(1, strlen(line) * sizeof(char) + 1);
        strncpy(bst_seats[i], line, strlen(line));
    }

    fclose(input_file);
    return bst_seats;
}

int bst_search(char* bst, char l, char u, int range) {
    int element = range / 2;
    int counter = 4;

    for (int i = 0; i < strlen(bst); i++) {
        if (bst[i] == l) {
            if (counter > range) {
                element--;
            }
            else {
                element -= (range / counter);
            }
            counter *= 2;
        }
        else if (bst[i] == u) {
            if (counter <= range) {
                element += (range/ counter);
            }
            counter *= 2;
        }   
    }

    return element;
}

int fill_seatmap(char seatmap[ROWS][COLUMNS], char* bst_seat)
{
    int row = bst_search(bst_seat, 'F', 'B', ROWS);
    int column = bst_search(bst_seat, 'L', 'R', COLUMNS);

    seatmap[row][column] = 1;

    return 1;
}

int main()
{
    int entries = 0;
    char** bst_seats = NULL;
    
    bst_seats = read_bst_seats(INPUT_FILENAME, &entries);

    char seatmap[ROWS][COLUMNS] = {{ 0 }};

    for (int i = 0; i < entries; i++) {
        fill_seatmap(seatmap, bst_seats[i]);
    }

    for (int row = 0; row < ROWS; row++) {
        for (int column = 0; column < COLUMNS; column++) {
            if (seatmap[row][column] == 0) {
                printf("%d %d %d\n", row, column, calculate_sid(row, column));
            }
        }
    }

    for (int i = 0; i < entries; i++) {
        free(bst_seats[i]);
    }
    free(bst_seats);

    return 0;
}