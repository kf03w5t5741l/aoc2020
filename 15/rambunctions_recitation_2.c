// Implements simple binary search tree. I wanted to avoid
// another hashtable as I've already implemented them twice
// for aoc2020. It works, but is very slow (60s on my machine).
// When I have time, I'd like to update this implementation
// to an AVL or red-black tree - hopefully balancing will
// speed things up.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TARGET_TURN 30000000

struct BstNode {
    int key;
    int position;
    struct BstNode* gt;
    struct BstNode* lt;
};

struct BstNode* bst_search(struct BstNode* bst, struct BstNode* search_node)
{
    if (bst == NULL) {
        return NULL;
    }

    if (search_node == NULL) {
        return NULL;
    }

    if (search_node->key == bst->key) {
        return bst;
    }

    if (search_node->key > bst->key) {
        return bst_search(bst->gt, search_node);
    }
    else {
        return bst_search(bst->lt, search_node);
    }

}

struct BstNode* bst_insert(struct BstNode* bst, struct BstNode* new_node)
{
    if (bst == NULL) {
        return new_node;
    }

    if (new_node->key == bst->key) {
        bst->position = new_node->position;
        free(new_node);
        return bst;
    }

    if (new_node->key > bst->key) {
        if (bst->gt == NULL) {
            bst->gt = new_node;
            return bst->gt;
        }
        else {
            return bst_insert(bst->gt, new_node);
        }
    }
    else {
        if (bst->lt == NULL) {
            bst->lt = new_node;
            return bst->lt;
        }
        else {
            return bst_insert(bst->lt, new_node);
        }
    }
}

struct BstNode* play_turn(struct BstNode* bst, struct BstNode* prev_turn)
{
    if (bst == NULL) {
        return NULL;
    }

    struct BstNode* search_result = bst_search(bst, prev_turn);

    if (search_result == NULL) {
        return bst_insert(bst, prev_turn);
    }

    if (search_result->position == prev_turn->position) {
        struct BstNode* turn = calloc(1, sizeof(struct BstNode*));
        
        turn->key = 0;
        turn->position = prev_turn->position + 1;
        //free(prev_turn);

        return turn;
    }
    else {
        struct BstNode* turn = calloc(1, sizeof(struct BstNode*));

        turn->key = prev_turn->position - search_result->position;
        turn->position = prev_turn->position + 1;

        search_result->position = prev_turn->position;
        free(prev_turn);

        return turn;
    }
    return NULL;
}

void bst_free(struct BstNode* bst)
{
    if (bst == NULL) {
        return;
    }

    bst_free(bst->gt);
    bst_free(bst->lt);

    free(bst);
}

int main()
{
    struct BstNode* bst = NULL;
    struct BstNode* prev_turn = NULL;

    char buffer[BUFSIZ] = { '\0' };

    int position = 1;

    while (fgets(buffer, BUFSIZ, stdin)){

        char* csv = strtok(buffer, ",");

        while (csv != NULL) {
            int value = atoi(csv);

            struct BstNode* new_node = calloc(1, sizeof(struct BstNode));
            new_node->key = value;
            new_node->position = position;
            prev_turn = bst_insert(bst, new_node);
            if (bst == NULL) {
                bst = prev_turn;
            }

            csv = strtok(NULL, ",");
            printf("Storing starting number %d: %d\n", prev_turn->position, prev_turn->key);
            position++;
        }
    }

    while (prev_turn->position < TARGET_TURN) {
        prev_turn = play_turn(bst, prev_turn);
        if (prev_turn->position % 1000000 == 0) {
            printf("Playing position %d: %d\n", prev_turn->position, prev_turn->key);
        }
    }
    printf("Playing position %d: %d\n", prev_turn->position, prev_turn->key);

    bst_free(bst);

    return 0;
}