#include "../include/ds.h"

void stack_init(Stack *s) {
    s->top = -1;
}

bool stack_is_empty(Stack *s) {
    return s->top == -1;
}

bool stack_is_full(Stack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}

bool stack_push(Stack *s, int item) {
    if (stack_is_full(s)) {
        return false;
    }
    s->items[++s->top] = item;
    return true;
}

int stack_pop(Stack *s) {
    if (stack_is_empty(s)) {
        return -1; // Should handle error appropriately
    }
    return s->items[s->top--];
}

int stack_peek(Stack *s) {
    if (stack_is_empty(s)) {
        return -1; // Should handle error appropriately
    }
    return s->items[s->top];
}
