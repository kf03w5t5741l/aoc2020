// Implements red-black tree
// TODO: update insertion function, add insertion fix function

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TARGET_TURN 30000000

enum COLOUR {
    RED,
    BLACK
};

struct RbtNode {
    int key;
    struct RbtNode* parent;
    struct RbtNode* left;
    struct RbtNode* right;
    enum COLOUR colour;

    int position;
};

struct RbtNode* rbt_search(struct RbtNode* rbt, struct RbtNode* search_node)
{
    if (rbt == NULL) {
        return NULL;
    }

    if (search_node == NULL) {
        return NULL;
    }

    if (search_node->key == rbt->key) {
        return rbt;
    }

    if (search_node->key > rbt->key) {
        return rbt_search(rbt->right, search_node);
    }
    else {
        return rbt_search(rbt->left, search_node);
    }

}

struct RbtNode* rbt_left_rotate(struct RbtNode** rbt, struct RbtNode* x)
{
    struct RbtNode* y = x->right;
    x->right = y->left;
    
    if (y->left != NULL) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;

    if (x->parent = NULL) {
        *rbt = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;

    return rbt;
}

struct RbtNode* rbt_right_rotate(struct RbtNode** rbt, struct RbtNode* y)
{
    struct RbtNode* x = y->left;
    y->left = x->right;
    
    if (x->right != NULL) {
        x->right->parent = y;
    }
    
    x->parent = y->parent;

    if (y->parent = NULL) {
        *rbt = x;
    }
    else if (y == y->parent->right) {
        y->parent->right = x;
    }
    else {
        y->parent->left = x;
    }

    x->left = y;
    y->parent = x;

    return rbt;
}

struct RbtNode* rbt_insert(struct RbtNode* rbt, struct RbtNode* new_node)
{
    if (rbt == NULL) {
        return new_node;
    }

    if (new_node->key == rbt->key) {
        rbt->position = new_node->position;
        free(new_node);
        return rbt;
    }

    if (new_node->key > rbt->key) {
        if (rbt->right == NULL) {
            rbt->right = new_node;
            return rbt->right;
        }
        else {
            return rbt_insert(rbt->right, new_node);
        }
    }
    else {
        if (rbt->left == NULL) {
            rbt->left = new_node;
            return rbt->left;
        }
        else {
            return rbt_insert(rbt->left, new_node);
        }
    }
}

struct RbtNode* play_turn(struct RbtNode* rbt, struct RbtNode* prev_turn)
{
    if (rbt == NULL) {
        return NULL;
    }

    struct RbtNode* search_result = rbt_search(rbt, prev_turn);

    if (search_result == NULL) {
        return rbt_insert(rbt, prev_turn);
    }

    if (search_result->position == prev_turn->position) {
        struct RbtNode* turn = calloc(1, sizeof(struct RbtNode*));
        
        turn->key = 0;
        turn->position = prev_turn->position + 1;
        //free(prev_turn);

        return turn;
    }
    else {
        struct RbtNode* turn = calloc(1, sizeof(struct RbtNode*));

        turn->key = prev_turn->position - search_result->position;
        turn->position = prev_turn->position + 1;

        search_result->position = prev_turn->position;
        free(prev_turn);

        return turn;
    }
    return NULL;
}

void rbt_free(struct RbtNode* rbt)
{
    if (rbt == NULL) {
        return;
    }

    rbt_free(rbt->right);
    rbt_free(rbt->left);

    free(rbt);
}

int main()
{
    struct RbtNode* rbt = NULL;
    struct RbtNode* prev_turn = NULL;

    char buffer[BUFSIZ] = { '\0' };

    int position = 1;

    while (fgets(buffer, BUFSIZ, stdin)){

        char* csv = strtok(buffer, ",");

        while (csv != NULL) {
            int value = atoi(csv);

            struct RbtNode* new_node = calloc(1, sizeof(struct RbtNode));
            new_node->key = value;
            new_node->position = position;
            prev_turn = rbt_insert(rbt, new_node);
            if (rbt == NULL) {
                rbt = prev_turn;
            }

            csv = strtok(NULL, ",");
            printf("Storing starting number %d: %d\n", prev_turn->position, prev_turn->key);
            position++;
        }
    }

    while (prev_turn->position < TARGET_TURN) {
        prev_turn = play_turn(rbt, prev_turn);
        if (prev_turn->position % 1000000 == 0) {
            printf("Playing position %d: %d\n", prev_turn->position, prev_turn->key);
        }
    }
    printf("Playing position %d: %d\n", prev_turn->position, prev_turn->key);

    rbt_free(rbt);

    return 0;
}