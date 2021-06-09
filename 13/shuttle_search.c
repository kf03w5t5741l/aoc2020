#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Bus {
    int id;
    struct Bus* next;
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
    struct Bus* bus_list_end = bus_list;
    char* token = strtok(buffer, ",");

    while (token != NULL ){
        struct Bus* new_bus = malloc(sizeof(struct Bus));
        new_bus->id = atoi(token);
        new_bus->next = NULL;

        if (bus_list == NULL) {
            new_bus->prev = NULL;
            bus_list = new_bus;
        }
        else {
            new_bus->prev = bus_list_end;
            bus_list_end->next = new_bus;
        }
        bus_list_end = new_bus;

        token = strtok(NULL, ",");
    }


    // part1: find earliest bus for the airport with given timestamp
    struct Bus* bus = bus_list;
    struct Bus* earliest_bus = NULL;
    int shortest_wait = 0;

    while(bus != NULL) {
        if (bus->id == 0) {
            bus = bus->next;
            continue;
        }

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
        bus = bus->next;
    }
    printf("earliest bus: %d\twait time: %d minutes\tsolution1: %d\n", earliest_bus->id, shortest_wait, earliest_bus->id * shortest_wait);


    // part2: find earliest timestamp allowing for consecutive departures
    // 
    // i.e. solve a system of linear Diophantine equations
    //
    // method: cascading (i.e. solve linear system by two equations at a time,
    //         then solve the result equation against the following equation, etc.
    //         finding solutions iteratively
    //
    //         approaches with Chinese remainder theorem and/or least common multiple
    //         would have been better. disliked the idea of being limited to primes
    //         though.
    //
    // resources: https://en.wikipedia.org/wiki/Diophantine_equation#Examples
    //            https://brilliant.org/wiki/linear-diophantine-equations-one-equation/  
    //            https://brilliant.org/wiki/system-of-linear-diophantine-equations/#diophantine-systems-with-restrictions    

    unsigned long long t = 0;
    bus = bus_list;

    int bus_position = 0;
    long double coeff = 0.0;
    long double cons = 0.0;

    while (bus != NULL) {
        //printf("Reached bus [#%d] %d:\n", bus_position, bus->id);
        if (bus->prev == NULL) {
            coeff = bus->id;
        }
        else if (bus->id != 0)
        {
            long double coeff_a = coeff;
            long double coeff_b = (long double) bus->id;
            long double cons_t = cons + (long double) bus_position;

            coeff_a /= coeff_b;
            cons_t /= coeff_b;

            long double int_sol = 0;
            
            while (fabsl(ceill(coeff_a * int_sol + cons_t)) - (coeff_a * int_sol + cons_t) > 0.0000000000001L
                    && fabsl(floorl(coeff_a * int_sol + cons_t)) - (coeff_a * int_sol + cons_t) < 0.0000000000001L) {
                //printf("\ttrying: %Lf * %Lf + %Lf = %Lf\n", coeff_a, int_sol, cons_t, (coeff_a * int_sol + cons_t));
                int_sol++;
            }
            //printf("\tFound: %Lf * %Lf + %Lf = %Lf\n", coeff_a, int_sol, cons_t, (coeff_a * int_sol + cons_t));

            int gcd = 0;
            for (int i = 1; i <= coeff && i <= coeff_b; i++) {
                if ((long long) coeff % i == 0 && (long long) coeff_b % i == 0) {
                    gcd = i;
                }
            }
            //printf("\tgcd(%Lf, %Lf) = %d\n", coeff, coeff_b, gcd);
            coeff_b /= gcd;
            cons_t = int_sol;
            //printf("\tv = %Lf * n + %Lf\n", coeff_b, cons_t);
            
            cons_t = cons_t * coeff;
            coeff *= coeff_b;
            cons = cons_t + cons;
            //printf("\tt = %Lf * n + %Lf\n", coeff, cons);
        }

        bus = bus->next;
        bus_position++;
    }
    printf("solution2: %.1Lf\n", cons);

    while (bus_list != NULL) {
        struct Bus* next_bus = bus_list->next;
        free(bus_list);
        bus_list = next_bus;
    }

    return 0;
}