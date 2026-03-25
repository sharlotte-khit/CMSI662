#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdbool.h>

#define MAX_STRING_LEN   ((size_t)1048576)
#define MAX_CAPACITY     ((size_t)(1024 * 1024))
#define DEFAULT_CAPACITY ((size_t)16)
#define GROWTH_FACTOR    ((size_t)2)

typedef enum {
    SUCCESS = 0,
    ERR_NULL_ARG,
    ERR_ALLOC,
    ERR_EMPTY,
    ERR_CAPACITY_OVERFLOW,
    ERR_INVALID_CAPACITY,
    ERR_STRING_TOO_LONG,
    ERR_MAX_CAPACITY
} Status;

typedef struct Stack Stack;

typedef struct {
    Status status;
    char *value;
} StringResult;

typedef struct {
    Status status;
    size_t value;
} SizeResult;

typedef struct {
    Status status;
    bool value;
} BoolResult;

const char *status_message(Status status);
Status stack_create(size_t initial_capacity, Stack **out);
Status stack_destroy(Stack **stack_ptr);
Status stack_push(Stack *stack, const char *str);
StringResult stack_pop(Stack *stack);
StringResult stack_peek(const Stack *stack);
SizeResult stack_size(const Stack *stack);
SizeResult stack_capacity(const Stack *stack);
BoolResult stack_is_empty(const Stack *stack);

#endif
