#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TARGET_TURN 2020

struct Node {
    int value;
    int position;
    struct Node* prev;
};

struct Node* play_turn(struct Node* list)
{
    if (list == NULL) {
        return NULL;
    }

    struct Node* new_node = malloc(sizeof(struct Node));
    new_node->value = 0;
    new_node->position = list->position + 1;
    new_node->prev = list;

    int value_to_consider = list->value;

    while (list->prev != NULL) {
        if (list->prev->value == value_to_consider) {
            new_node->value = new_node->prev->position - list->prev->position;

            // remove previous occurrence from list
            struct Node* old_node = list->prev;
            list->prev = list->prev->prev;

            if (old_node != NULL) {
                free(old_node);
            }

            break;
        }
        list = list->prev;
    }

    return new_node;
}

struct Node* list_append(struct Node* list, int value)
{
    struct Node* node = malloc(sizeof(struct Node));
    node->value = value;

    if (list == NULL) {
        node->position = 1;
        node->prev = NULL;
        return node;
    }

    node->position = list->position + 1;
    node->prev = list;

    return node;
}

void list_free(struct Node* list)
{
    if (list == NULL) {
        return;
    }

    while (list->prev != NULL) {
        list_free(list->prev);
    }

    free(list);
}

int main()
{
    struct Node* list = NULL;
    char buffer[BUFSIZ] = { '\0' };

    while (fgets(buffer, BUFSIZ, stdin)){

        char* csv = strtok(buffer, ",");

        while (csv != NULL) {
            int value = atoi(csv);
            list = list_append(list, value);
            csv = strtok(NULL, ",");
            printf("Storing starting number %d: %d\n", list->position, list->value);
        }
    }

    while (list->position < TARGET_TURN) {
        list = play_turn(list);

        if (list->position % 10000 == 0) {
            printf("Playing position %d: %d\n", list->position, list->value);
        }
    }
    printf("Playing position %d: %d\n", list->position, list->value);

    list_free(list);

    return 0;
}