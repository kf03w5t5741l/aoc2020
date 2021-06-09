#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Bus {
    int id;
    struct Bus* prev;
};

int main()
{
    int timestamp;
    scanf("%d\n", &timestamp);
    printf("timestamp: %d\n", timestamp);

    char buffer[BUFSIZ];
    char* line = fgets(buffer, BUFSIZ, stdin);

    struct Bus* bus_list = NULL;
    char* token = strtok(buffer, ",");

    while (token != NULL ){
        if (token[0] != 'x') {
            struct Bus* new_bus = malloc(sizeof(struct Bus));
            new_bus->id = atoi(token);
            new_bus->prev = bus_list;
            bus_list = new_bus;
        }

        token = strtok(NULL, ",");
    }


    // find earliest bus for the airport with given timestamp
    struct Bus* bus = bus_list;
    struct Bus* earliest_bus = NULL;
    int shortest_wait = 0;

    while(bus != NULL) {
        int bus_timestamp = timestamp;

        while (bus_timestamp % bus->id != 0) {
            bus_timestamp++;
        }

        int wait = bus_timestamp - timestamp;

        if (earliest_bus == NULL || (bus_timestamp - timestamp) < shortest_wait) {
            earliest_bus = bus;
            shortest_wait = wait;
        }

        printf("%d: %d\n", bus->id, wait);
        bus = bus->prev;
    }
    printf("earliest bus: %d\twait time: %d minutes\tsolution1: %d\n", earliest_bus->id, shortest_wait, earliest_bus->id * shortest_wait);

    while (bus_list != NULL) {
        struct Bus* prev_bus = bus_list->prev;
        free(bus_list);
        bus_list = prev_bus;
    }

    return 0;
}