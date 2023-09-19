#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define the non-terminals and terminals
enum { E, T, F, ADD, SUB, MUL, DIV, OPEN, CLOSE, ID, NUM, EPS, END };

// Define the grammar rules
int grammar[][12] = {
        {T, ADD, E, END},
        {T, SUB, E, END},
        {T, END},
        {F, MUL, T, END},
        {F, DIV, T, END},
        {F, END},
        {OPEN, E, CLOSE, END},
        {ID, END},
        {NUM, END},
        {EPS, END}
};

// Define FIRST and FOLLOW sets
int FIRST[12][12] = {
        {T, OPEN, ID, NUM, EPS, -1},
        {ADD, SUB, -1},
        {F, OPEN, ID, NUM, EPS, -1},
        {MUL, DIV, -1},
        {OPEN, -1},
        {ID, -1},
        {NUM, -1},
        {EPS, -1}
};

int FOLLOW[12][12] = {
        {ADD, SUB, CLOSE, END, -1},
        {T, ADD, SUB, CLOSE, END, -1},
        {MUL, DIV, ADD, SUB, CLOSE, END, -1},
        {F, MUL, DIV, ADD, SUB, CLOSE, END, -1},
        {OPEN, ID, NUM, -1},
        {T, ADD, SUB, CLOSE, END, -1},
        {F, MUL, DIV, ADD, SUB, CLOSE, END, -1},
        {CLOSE, END, -1}
};

// Stack for parsing
int stack[1000];
int top = -1;

// Function to push a symbol onto the stack
void push(int symbol) {
    stack[++top] = symbol;
}

// Function to pop a symbol from the stack
int pop() {
    return stack[top--];
}

// Function to parse the input string
void parse(char* input) {
    int input_len = strlen(input);
    int index = 0;
    push(END);
    push(E);

    while (top >= 0) {
        int stack_top = stack[top];
        char lookahead = input[index];

        if (stack_top == EPS) {
            pop();
        } else if (stack_top == END) {
            if (lookahead == '\0') {
                printf("Parsing successful.\n");
                return;
            } else {
                printf("Syntax error at position %d: Unexpected symbol '%c'\n", index, lookahead);
                return;
            }
        } else if (stack_top >= ID && stack_top <= NUM) {
            if (stack_top == lookahead) {
                pop();
                index++;
            } else {
                printf("Syntax error at position %d: Expected symbol '%c'\n", index, stack_top);
                return;
            }
        } else {
            int rule = -1;
            for (int i = 0; grammar[stack_top][i] != END; i++) {
                if (FIRST[stack_top][i] == lookahead) {
                    rule = i;
                    break;
                }
            }

            if (rule == -1) {
                printf("Syntax error at position %d: Unexpected symbol '%c'\n", index, lookahead);
                return;
            }

            pop();

            for (int i = 0; grammar[stack_top][i] != END; i++) {
                if (grammar[stack_top][i] != EPS) {
                    push(grammar[stack_top][i]);
                }
            }
        }
    }
}

int main() {
    char input[1000];
    printf("Enter an arithmetic expression: ");
    scanf("%s", input);
    parse(input);
    return 0;
}
