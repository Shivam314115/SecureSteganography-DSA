#ifndef DS_H
#define DS_H

#include <stdbool.h>

#define MAX_STACK_SIZE 100

typedef struct {
    int items[MAX_STACK_SIZE];
    int top;
} Stack;

void stack_init(Stack *s);
bool stack_is_empty(Stack *s);
bool stack_is_full(Stack *s);
bool stack_push(Stack *s, int item);
int stack_pop(Stack *s);
int stack_peek(Stack *s);

#endif // DS_H
