#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WAYPOINT_X_START 10.0
#define WAYPOINT_Y_START -1.0

struct Position {
    double x;
    double y;
};

int main() {

    struct Position ferry;
    ferry.x = 0.0;
    ferry.y = 0.0;

    struct Position waypoint;
    waypoint.x = WAYPOINT_X_START;
    waypoint.y = WAYPOINT_Y_START;

    while(1) {
        char buffer[BUFSIZ] = { '\0' };
        char* line = fgets(buffer, BUFSIZ, stdin);

        if (line == NULL) {
            break;
        }

        char operator = buffer[0];
        int operand = atoi(buffer + 1);

        double d_y = waypoint.y - ferry.y;
        double d_x = waypoint.x - ferry.x;
        double theta = 0;
        double magnitude = sqrt(pow((double) d_y, 2) + pow((double) d_x, 2));

        switch (operator) {
            case 'L':
                theta = -1.0 * operand / 180.0f * M_PI;
                waypoint.x = ferry.x + (d_x * cos(theta) - d_y * sin(theta));
                waypoint.y = ferry.y + (d_x * sin(theta) + d_y * cos(theta));
                break;
            case 'R':
                theta = operand / 180.0f * M_PI;
                waypoint.x = ferry.x + (d_x * cos(theta) - d_y * sin(theta));
                waypoint.y = ferry.y + (d_x * sin(theta) + d_y * cos(theta));
                break;

            case 'N':
                waypoint.y -= operand;
                break;
            case 'S':
                waypoint.y += operand;
                break;
            case 'E':
                waypoint.x += operand;
                break;
            case 'W':
                waypoint.x -= operand;
                break;

            case 'F':
                ferry.x  += operand * d_x;
                ferry.y  += operand * d_y;

                waypoint.x += operand * d_x;
                waypoint.y += operand * d_y;
                break;

        }
        d_y = waypoint.y - ferry.y;
        d_x = waypoint.x - ferry.x;
        magnitude = sqrt(pow((double) d_y, 2) + pow((double) d_x, 2));
        printf("[%c%3d]\tx: %3.1f\ty: %3.1f\tx: %3.1f\ty: %3.1f\ttheta: % 1.1f * pi\tmagnitude: %f\n", operator, operand, ferry.x, ferry.y, waypoint.x, waypoint.y, theta / M_PI, magnitude);
    }

    int manhattan = abs((int) ferry.x) + abs((int) ferry.y);
    printf("manhattan: %d\n", manhattan);

    return 0;
}