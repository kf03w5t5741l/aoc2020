#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Ship {
    int x;
    int y;
    double direction;
};

int main() {

    struct Ship ferry;
    ferry.x = 0;
    ferry.y = 0;
    ferry.direction = M_PI / 2;

    while(1) {
        char buffer[BUFSIZ] = { '\0' };
        char* line = fgets(buffer, BUFSIZ, stdin);

        if (line == NULL) {
            break;
        }

        char operator = buffer[0];
        int operand = atoi(buffer + 1);

        switch (operator) {
            case 'L':
                ferry.direction = fmod((ferry.direction - (double) operand / 180.0 * M_PI), 2 * M_PI);
                break;
            case 'R':
                ferry.direction = fmod((ferry.direction + (double) operand / 180.0 * M_PI), 2 * M_PI);
                break;

            case 'N':
                ferry.y -= operand;
                break;
            case 'S':
                ferry.y += operand;
                break;
            case 'E':
                ferry.x += operand;
                break;
            case 'W':
                ferry.x -= operand;
                break;

            case 'F':
                ferry.x  += operand * (int) sin(ferry.direction);
                ferry.y  -= operand * (int) cos(ferry.direction);
                break;

        }

        printf("[%c%3d]\tx: %3d\ty: %3d\tdirection: %1.1f * pi\n", operator, operand, ferry.x, ferry.y, ferry.direction / M_PI);
    }

    int manhattan = abs(ferry.x) + abs(ferry.y);
    printf("manhattan: %d\n", manhattan);

    return 0;
}