#include <stdio.h>

#define ITERATIONS 6

#define START_SIZE_LENGTH 3

struct Position {
    int x;
    int y;
    int z;
};

void deactivate_cube(
    int axis_length,
    char world[axis_length][axis_length][axis_length],
    char new_world[axis_length][axis_length][axis_length],
    int x,
    int y,
    int z)
{
    world[x][y][z] = '.';
}

int validate_coordinate(int axis_length, int coordinate)
{
    coordinate = coordinate >= 0 ? coordinate : 0;
    coordinate = coordinate < axis_length ? coordinate : axis_length - 1;
    return coordinate;
}

void do_for_every_cube(
    int axis_length,
    char world[axis_length][axis_length][axis_length],
    char new_world[axis_length][axis_length][axis_length],
    void fn())
{
    for (int x = 0; x < axis_length; x++) {
        for (int y = 0; y < axis_length; y++) {
            for (int z = 0; z < axis_length; z++) {
                fn(axis_length, world, new_world, x, y, z);
            }
        }
    }
}

void copy_cube(
    int axis_length,
    char world[axis_length][axis_length][axis_length],
    char new_world[axis_length][axis_length][axis_length],
    int x,
    int y,
    int z)
{
    new_world[x][y][z] = world[x][y][z];
}

void print_z_layers(
    int axis_length,
    char world[axis_length][axis_length][axis_length])
{
    for (int z = 0; z < axis_length; z++) {
        printf("z=%d\n", z);
        for (int x = 0; x < axis_length; x++) {
            for (int y = 0; y < axis_length; y++) {
                printf("%c", world[x][y][z]);
            }
            printf("\n");
        }
    }
}

void apply_rules_to_cube(
    int axis_length,
    char world[axis_length][axis_length][axis_length],
    char new_world[axis_length][axis_length][axis_length],
    int cube_x,
    int cube_y,
    int cube_z)
{
    int x_min = validate_coordinate(axis_length, cube_x - 1);
    int x_max = validate_coordinate(axis_length, cube_x + 1);
    
    int y_min = validate_coordinate(axis_length, cube_y - 1);
    int y_max = validate_coordinate(axis_length, cube_y + 1);

    int z_min = validate_coordinate(axis_length, cube_z - 1);
    int z_max = validate_coordinate(axis_length, cube_z + 1);

    int active_neighbours = 0;

    // count all neighbouring active cubes
    for (int x = x_min; x<= x_max; x++) {
        for (int y = y_min; y <= y_max; y++) {
            for (int z = z_min; z <= z_max; z++) {

                // skip cube that rules are being applied to
                if (x == cube_x && y == cube_y && z == cube_z) {
                    continue;
                }

                if (world[x][y][z] == '#') {
                    active_neighbours++;
                }
            }
        }
    }

    char updated_cube = world[cube_x][cube_y][cube_z];

    switch (world[cube_x][cube_y][cube_z])
    {
        case '#':
        {
            if (active_neighbours != 2 && active_neighbours != 3) {
                updated_cube = '.';
            }
            break;
        }
        case '.':
        {
            if (active_neighbours == 3) {
                updated_cube = '#';
            }
            break;
        }
    }

    new_world[cube_x][cube_y][cube_z] = updated_cube;
}

void apply_rules(
    int axis_length,
    char world[axis_length][axis_length][axis_length])
{
    char new_world[axis_length][axis_length][axis_length];
    do_for_every_cube(axis_length, world, new_world, &copy_cube);
    do_for_every_cube(axis_length, new_world, world, &apply_rules_to_cube);
}

void populate_world(
    int axis_length,
    char world[axis_length][axis_length][axis_length],
    struct Position origin)
{
    int x_pos = origin.x;

    char buffer[BUFSIZ];

    while (fgets(buffer, BUFSIZ, stdin)) {
        int buf_index = 0;

        while((buffer[buf_index] == '#' || buffer[buf_index] == '.')
               && buf_index < axis_length / 2) {
            printf("Inserting '%c' at x: %d y: %d z: %d\n", buffer[buf_index], x_pos, buf_index + origin.y, origin.z);
            world[x_pos][origin.y + buf_index][origin.z] = buffer[buf_index];
            buf_index++;
        }
        x_pos++;
    }
}

int count_active_cubes(
    int axis_length,
    char world[axis_length][axis_length][axis_length])
{
    int active_cubes = 0;

    for (int x = 0; x < axis_length; x++) {
        for (int y = 0; y < axis_length; y++) {
            for (int z = 0; z < axis_length; z++) {
                if (world[x][y][z] == '#') {
                    active_cubes++;
                }
            }
        }
    }

    return active_cubes;
}

int main()
{
    const int axis_length = 50;//START_SIZE_LENGTH +  2 * ITERATIONS;

    struct Position origin;
    origin.x = axis_length / 2;
    origin.y = axis_length / 2;
    origin.z = axis_length / 2;

    char world[axis_length][axis_length][axis_length];
    do_for_every_cube(axis_length, world, NULL, &deactivate_cube);

    populate_world(axis_length, world, origin);

    printf("x, y, z axis lengths: %d cubes\n", axis_length);

    printf("Old world:\n");
    print_z_layers(axis_length, world);

    for (int i = 0; i < ITERATIONS; i++) {
        apply_rules(axis_length, world);
        printf("New world: iteration %d\n", i);
        print_z_layers(axis_length, world);
    }

    int active_cubes = count_active_cubes(axis_length, world);
    printf("part1: %d\n", active_cubes);

    return 0;
}
