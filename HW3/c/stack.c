#include "stack.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct Stack {
    char **data;
    size_t size;
    size_t capacity;
};

static void zero_mem(void *ptr, size_t len)
{
    volatile unsigned char *p = (volatile unsigned char *)ptr;
    while (len--) {
        *p++ = 0;
    }
}

static void free_str(char *str)
{
    if (str != NULL) {
        zero_mem(str, strlen(str));
        free(str);
    }
}

static char *copy_str(const char *src)
{
    if (src == NULL) {
        return NULL;
    }

    size_t len = strlen(src);
    char *copy = malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, src, len + 1);
    return copy;
}

const char *status_message(Status status)
{
    switch (status) {
        case SUCCESS: return "Success";
        case ERR_NULL_ARG: return "Null argument provided";
        case ERR_ALLOC: return "Memory allocation failed";
        case ERR_EMPTY: return "Stack is empty";
        case ERR_CAPACITY_OVERFLOW: return "Capacity calculation overflowed";
        case ERR_INVALID_CAPACITY: return "Invalid initial capacity";
        case ERR_STRING_TOO_LONG: return "String exceeds maximum allowed length";
        case ERR_MAX_CAPACITY: return "Stack has reached max capacity";
        default: return "Unknown error";
    }
}

Status stack_create(size_t initial_capacity, Stack **out)
{
    if (out == NULL) {
        return ERR_NULL_ARG;
    }
    *out = NULL;

    if (initial_capacity == 0) {
        initial_capacity = DEFAULT_CAPACITY;
    }

    if (initial_capacity > MAX_CAPACITY) {
        return ERR_INVALID_CAPACITY;
    }

    if (initial_capacity > SIZE_MAX / sizeof(char *)) {
        return ERR_CAPACITY_OVERFLOW;
    }

    Stack *stack = calloc(1, sizeof(Stack));
    if (stack == NULL) {
        return ERR_ALLOC;
    }

    stack->data = calloc(initial_capacity, sizeof(char *));
    if (stack->data == NULL) {
        zero_mem(stack, sizeof(Stack));
        free(stack);
        return ERR_ALLOC;
    }

    stack->size = 0;
    stack->capacity = initial_capacity;
    *out = stack;

    return SUCCESS;
}

Status stack_destroy(Stack **stack_ptr)
{
    if (stack_ptr == NULL) {
        return ERR_NULL_ARG;
    }

    Stack *stack = *stack_ptr;
    if (stack == NULL) {
        return ERR_NULL_ARG;
    }

    for (size_t i = 0; i < stack->size; i++) {
        free_str(stack->data[i]);
        stack->data[i] = NULL;
    }

    zero_mem(stack->data, stack->capacity * sizeof(char *));
    free(stack->data);

    zero_mem(stack, sizeof(Stack));
    free(stack);

    *stack_ptr = NULL;

    return SUCCESS;
}

static Status expand(Stack *stack)
{
    if (stack->capacity >= MAX_CAPACITY) {
        return ERR_MAX_CAPACITY;
    }

    size_t new_capacity;
    if (stack->capacity > MAX_CAPACITY / GROWTH_FACTOR) {
        new_capacity = MAX_CAPACITY;
    } else {
        new_capacity = stack->capacity * GROWTH_FACTOR;
    }

    if (new_capacity > SIZE_MAX / sizeof(char *)) {
        return ERR_CAPACITY_OVERFLOW;
    }

    char **new_data = realloc(stack->data, new_capacity * sizeof(char *));
    if (new_data == NULL) {
        return ERR_ALLOC;
    }

    memset(new_data + stack->capacity, 0,
           (new_capacity - stack->capacity) * sizeof(char *));

    stack->data = new_data;
    stack->capacity = new_capacity;

    return SUCCESS;
}

Status stack_push(Stack *stack, const char *str)
{
    if (stack == NULL || str == NULL) {
        return ERR_NULL_ARG;
    }

    size_t len = 0;
    while (len <= MAX_STRING_LEN && str[len] != '\0') {
        len++;
    }
    if (len > MAX_STRING_LEN) {
        return ERR_STRING_TOO_LONG;
    }

    if (stack->size >= stack->capacity) {
        Status status = expand(stack);
        if (status != SUCCESS) {
            return status;
        }
    }

    char *copy = malloc(len + 1);
    if (copy == NULL) {
        return ERR_ALLOC;
    }
    memcpy(copy, str, len);
    copy[len] = '\0';

    stack->data[stack->size] = copy;
    stack->size++;

    return SUCCESS;
}

StringResult stack_pop(Stack *stack)
{
    StringResult result = { SUCCESS, NULL };

    if (stack == NULL) {
        result.status = ERR_NULL_ARG;
        return result;
    }

    if (stack->size == 0) {
        result.status = ERR_EMPTY;
        return result;
    }

    stack->size--;
    result.value = stack->data[stack->size];
    stack->data[stack->size] = NULL;

    return result;
}

StringResult stack_peek(const Stack *stack)
{
    StringResult result = { SUCCESS, NULL };

    if (stack == NULL) {
        result.status = ERR_NULL_ARG;
        return result;
    }

    if (stack->size == 0) {
        result.status = ERR_EMPTY;
        return result;
    }

    result.value = copy_str(stack->data[stack->size - 1]);
    if (result.value == NULL) {
        result.status = ERR_ALLOC;
    }

    return result;
}

SizeResult stack_size(const Stack *stack)
{
    SizeResult result = { SUCCESS, 0 };

    if (stack == NULL) {
        result.status = ERR_NULL_ARG;
        return result;
    }

    result.value = stack->size;
    return result;
}

SizeResult stack_capacity(const Stack *stack)
{
    SizeResult result = { SUCCESS, 0 };

    if (stack == NULL) {
        result.status = ERR_NULL_ARG;
        return result;
    }

    result.value = stack->capacity;
    return result;
}

BoolResult stack_is_empty(const Stack *stack)
{
    BoolResult result = { SUCCESS, true };

    if (stack == NULL) {
        result.status = ERR_NULL_ARG;
        return result;
    }

    result.value = (stack->size == 0);
    return result;
}
