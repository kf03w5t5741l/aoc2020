#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILENAME "./input"

struct Group {
    char* questions;
    struct Group* next;
};

int count_common_questions(struct Group* group)
{
    int common_questions = 0;

    for (int i = 0; i < 26; i++) {
        if (group->questions[i] > 0) {
            common_questions++;
        }
    }

    return common_questions;
}

void free_groups(struct Group* groups)
{
    if (groups == NULL) {
        return;
    }

    if (groups->questions != NULL) {
        free(groups->questions);
    }
    struct Group* next = groups->next;
    free(groups);

    free_groups(next);
}

struct Group* read_groups(char* filename)
{
    FILE* input_file = fopen(INPUT_FILENAME, "r");

    struct Group* group = calloc(1, sizeof(struct Group));
    group->questions = calloc(1, sizeof(char) * 26);
    struct Group* groups = group;

    while(!feof(input_file)) {
        char buffer[256] = { 0 };
        fgets(buffer, 255, input_file);

        if (buffer[0] == '\n') {
            group->next = calloc(1, sizeof(struct Group));
            group->next->questions = calloc(1, sizeof(char) * 26);
            group = group->next;
            continue;
        }

        for (int i = 0; i < strlen(buffer); i++) {
            char c = buffer[i];

            if (c >= 'a' && c <= 'z') {
                group->questions[c - 'a'] += 1;
            }
        }
    }

    group = groups;
    while (group != NULL) {
        for (int i = 0; i < 26; i++) {
        }
        group = group->next;
    }

    fclose(input_file);
    return groups;
}

int main(int argc, char* argv[]) {

    struct Group* groups = read_groups(INPUT_FILENAME);
    struct Group* group = groups;

    int common_questions = 0;
    while (group != NULL) {
        common_questions += count_common_questions(group);
        group = group->next;
    }
    printf("Common questions: %d\n", common_questions);

    free_groups(groups);

    return 0;
}