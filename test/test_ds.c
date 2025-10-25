#include "../include/ds.h"
#include <assert.h>
#include <stdio.h>

int main() {
    Stack s;
    stack_init(&s);

    assert(stack_is_empty(&s));
    assert(!stack_is_full(&s));

    stack_push(&s, 10);
    assert(!stack_is_empty(&s));
    assert(stack_peek(&s) == 10);

    stack_push(&s, 20);
    assert(stack_peek(&s) == 20);

    int item = stack_pop(&s);
    assert(item == 20);
    assert(stack_peek(&s) == 10);

    item = stack_pop(&s);
    assert(item == 10);
    assert(stack_is_empty(&s));

    printf("Stack test passed!\n");

    return 0;
}
