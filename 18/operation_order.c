#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StringNode {
    char* string;
    struct StringNode* next;
    struct StringNode* prev;
};

struct StringNode* read_strings()
{
    struct StringNode* string_list = NULL;
    struct StringNode* string_iterator = NULL;

    char buffer[BUFSIZ] = { '\0' };

    while (fgets(buffer, BUFSIZ, stdin)) {
        struct StringNode* string_node = calloc(1, sizeof(struct StringNode));
        string_node->string = strdup(buffer);

        if (string_iterator == NULL) {
            string_list = string_node;
            string_iterator = string_node;
        }
        else {
            string_iterator->next = string_node;
            string_iterator = string_node;
        }
    }

    return string_list;
}

void free_string_list(struct StringNode* string_node)
{
    if (string_node == NULL) {
        return;
    }

    if (string_node->next != NULL) {
        free_string_list(string_node->next);
    }

    if (string_node->string != NULL) {
        free(string_node->string);
    }

    free(string_node);
}

long long evaluate_expression_1(char* expression, int* offset)
{
    // conceptualise each expression as a nested sequence of pairs of terms
    long long result = 0;
    int read = 0;

    char first_term[BUFSIZ];
    char second_term[BUFSIZ];
    char operator = '\0';

    // parse first term and use as initial value
    sscanf(expression + *offset, "%s%n", first_term, &read);
    if (first_term[0] == '(') {
        *offset += 1;
        result = evaluate_expression_1(expression, offset);
    }
    else {
        result = strtoll(first_term, NULL, 10);
        *offset += read;
    }

    while (*offset + 4 < strlen(expression)
           && sscanf(expression + *offset, " %c %s%n", &operator, second_term, &read)) {
        long long operand = 0;
        if (operator == ')') {
            *offset += 1;
            return result;
        }
        if (second_term[0] == '(') {
            *offset += 4;
            operand = evaluate_expression_1(expression, offset);
        }
        else {
            operand = strtoll(second_term, NULL, 10);
            *offset += read;
        }

        switch (operator) {
            case '+':
                result += operand;
                break;
            case '-':
                result -= operand;
                break;
            case '*':
                result *= operand;
                break;
            case '/':
                result /= operand;
                break;
        }
        if (strchr(second_term, ')') != strrchr(second_term, (')'))) {
            *offset -= 1;
        }
        if (strchr(second_term, ')')) {
            return result;
        }
    }

    return result;
}

void free_string_node(struct StringNode* string_node)
{
    if (string_node == NULL) {
        return;
    }

    if (string_node->string != NULL) {
        free(string_node->string);
    }

    free(string_node);
} 

struct StringNode* evaluate_binomial(struct StringNode* term_list)
{
    
    struct StringNode* result_node = calloc(1, sizeof(struct StringNode));
    long long result = 0;
    
    switch (term_list->string[0]) {
        case '+':
            result = atoll(term_list->prev->string) + atoll(term_list->next->string);
            break;
        case '*':
            result = atoll(term_list->prev->string) * atoll(term_list->next->string);
            break;
    }
    
    result_node->string = calloc(BUFSIZ, sizeof(char));
    sprintf(result_node->string, "%lld", result);

    result_node->prev = term_list->prev->prev;
    result_node->next = term_list->next->next;

    if (result_node->prev != NULL) {
        result_node->prev->next = result_node;
    }

    if (result_node->next != NULL) {
        result_node->next->prev = result_node;
    }

    //free_string_node(term_list->prev);
    //free_string_node(term_list->next);
    //free_string_node(term_list);

    return result_node;
}

long long evaluate_expression_2(char* expression)
{
    struct StringNode* term_list = NULL;
    char term_str[BUFSIZ];
    int offset = 0;
    int read = 0;

    char* temp_expression = strdup(expression);
    char* token = NULL;
    char* token_str = temp_expression;

    while ((token = strtok(token_str, " "))) {

    }

    while (offset + 2 < strlen(expression)
           && sscanf(expression + offset, "%s%n", term_str, &read)) {
        struct StringNode* term = calloc(1, sizeof(struct StringNode));

        if (term_str[0] == '(') {
            term->string = calloc(2, sizeof(char));
            term->string[0] = term_str[0];
            offset += 1;
        }
        else {
            term->string = strdup(term_str);

            if (strchr(term->string, ')') && strchr(term->string, ')') != term->string) {
                int close_paren_pos = strchr(term->string, (')')) - term->string;
                term->string[close_paren_pos] = '\0';
                offset = offset + close_paren_pos + 1;
            }
            else {
                offset += read;
            }
        }

        if (term_list == NULL) {
            term_list = term;
        }
        else {
            term_list->next = term;
            term->prev = term_list;
            term_list = term;
        }

        printf("%s\n", term->string);
    }
    printf("\n");

    while (term_list->prev != NULL) {
        if (strchr(term_list->string, '(')) {
            struct StringNode* sub_expr = term_list;
            while (!strchr(sub_expr->string, ')')) {
                if (sub_expr->string[0] == '+') {
                    sub_expr = evaluate_binomial(sub_expr);
                }
                else {
                    sub_expr = sub_expr->next;
                }
            }
            sub_expr = term_list;
            while (!strchr(sub_expr->string, ')')) {
                if (sub_expr->string[0] == '*') {
                    sub_expr = evaluate_binomial(sub_expr);
                }
                else {
                    sub_expr = sub_expr->next;
                }
            }

            if (term_list->prev != NULL) {
                term_list->prev->next = term_list->next;
            }
            term_list->next->prev = term_list->prev;
            sub_expr->prev->next = sub_expr->next;
            if (sub_expr->next != NULL) {
                sub_expr->next->prev = sub_expr->prev;
            }
        }
        term_list = term_list->prev;
    }

    struct StringNode* sub_expr = term_list;

    while (sub_expr != NULL) {
        if (sub_expr->string[0] == '+') {
            sub_expr = evaluate_binomial(sub_expr);
        }
        else {
            sub_expr = sub_expr->next;
        }
    }

    sub_expr = term_list;

    while (sub_expr != NULL) {
        if (sub_expr->string[0] == '*') {
            sub_expr = evaluate_binomial(sub_expr);
        }
        else {
            sub_expr = sub_expr->next;
        }
    }

    return atoll(term_list->string);
}

int main()
{
    struct StringNode* expressions = read_strings();
    long long part1 = 0;
    long long part2 = 0;

    struct StringNode* expression = expressions;
    while(expression != NULL) {
        int offset = 0;

        long long result_1 = evaluate_expression_1(expression->string, &offset);

        offset = 0;
        long long result_2 = evaluate_expression_2(expression->string);

        printf("%s", expression->string);
        printf("\t= %lld\n", result_1);
        printf("\t= %lld\n", result_2);

        part1 += result_1;
        part2 += result_2;

        expression = expression->next;
    }

    printf("part1: %lld\n", part1);
    printf("part2: %lld\n", part2);

    free_string_list(expressions);

    return 0;
}