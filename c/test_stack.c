#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) \
    do { printf("  %-50s ", name); tests_run++; } while (0)

#define PASS() \
    do { tests_passed++; printf("PASS\n"); } while (0)

#define FAIL(msg) \
    do { printf("FAIL  (%s)\n", msg); } while (0)

#define ASSERT(cond, msg) \
    do { if (!(cond)) { FAIL(msg); return; } } while (0)

static void test_create_and_destroy(void)
{
    TEST("create with explicit capacity");
    Stack *s = NULL;
    Status st = stack_create(4, &s);
    ASSERT(st == SUCCESS, "expected SUCCESS");
    ASSERT(s != NULL, "expected non-NULL stack");
    stack_destroy(&s);
    ASSERT(s == NULL, "destroy should NULL the pointer");
    PASS();
}

static void test_create_default_capacity(void)
{
    TEST("create with 0 uses default capacity");
    Stack *s = NULL;
    Status st = stack_create(0, &s);
    ASSERT(st == SUCCESS, "expected SUCCESS");
    SizeResult cap = stack_capacity(s);
    ASSERT(cap.status == SUCCESS, "capacity call failed");
    ASSERT(cap.value == DEFAULT_CAPACITY, "expected DEFAULT_CAPACITY");
    stack_destroy(&s);
    PASS();
}

static void test_create_null_out(void)
{
    TEST("create with NULL out pointer");
    Status st = stack_create(4, NULL);
    ASSERT(st == ERR_NULL_ARG, "expected ERR_NULL_ARG");
    PASS();
}

static void test_create_excessive_capacity(void)
{
    TEST("create with capacity > MAX_CAPACITY");
    Stack *s = NULL;
    Status st = stack_create(MAX_CAPACITY + 1, &s);
    ASSERT(st == ERR_INVALID_CAPACITY, "expected ERR_INVALID_CAPACITY");
    ASSERT(s == NULL, "out should remain NULL on failure");
    PASS();
}

static void test_destroy_null_pointer(void)
{
    TEST("destroy with NULL pointer-to-pointer");
    Status st = stack_destroy(NULL);
    ASSERT(st == ERR_NULL_ARG, "expected ERR_NULL_ARG");
    PASS();
}

static void test_destroy_null_stack(void)
{
    TEST("destroy when *stack_ptr is already NULL");
    Stack *s = NULL;
    Status st = stack_destroy(&s);
    ASSERT(st == ERR_NULL_ARG, "expected ERR_NULL_ARG");
    PASS();
}

static void test_push_and_size(void)
{
    TEST("push three strings and check size");
    Stack *s = NULL;
    stack_create(2, &s);

    ASSERT(stack_push(s, "alpha") == SUCCESS, "push alpha");
    ASSERT(stack_push(s, "beta") == SUCCESS, "push beta");
    ASSERT(stack_push(s, "gamma") == SUCCESS, "push gamma (triggers expand)");

    SizeResult sz = stack_size(s);
    ASSERT(sz.status == SUCCESS, "size call failed");
    ASSERT(sz.value == 3, "expected size 3");

    stack_destroy(&s);
    PASS();
}

static void test_push_null_args(void)
{
    TEST("push with NULL stack or NULL string");
    Stack *s = NULL;
    stack_create(4, &s);

    ASSERT(stack_push(NULL, "x") == ERR_NULL_ARG, "NULL stack");
    ASSERT(stack_push(s, NULL) == ERR_NULL_ARG, "NULL string");

    stack_destroy(&s);
    PASS();
}

static void test_push_copies_input(void)
{
    TEST("push copies the input string");
    Stack *s = NULL;
    stack_create(4, &s);

    char buf[16];
    strcpy(buf, "original");
    stack_push(s, buf);

    strcpy(buf, "MODIFIED");

    StringResult r = stack_peek(s);
    ASSERT(r.status == SUCCESS, "peek failed");
    ASSERT(strcmp(r.value, "original") == 0, "internal copy should be unaffected");
    free(r.value);

    stack_destroy(&s);
    PASS();
}

static void test_pop_order(void)
{
    TEST("pop returns elements in LIFO order");
    Stack *s = NULL;
    stack_create(4, &s);
    stack_push(s, "first");
    stack_push(s, "second");
    stack_push(s, "third");

    StringResult r;

    r = stack_pop(s);
    ASSERT(r.status == SUCCESS, "pop 1 status");
    ASSERT(strcmp(r.value, "third") == 0, "expected 'third'");
    free(r.value);

    r = stack_pop(s);
    ASSERT(r.status == SUCCESS, "pop 2 status");
    ASSERT(strcmp(r.value, "second") == 0, "expected 'second'");
    free(r.value);

    r = stack_pop(s);
    ASSERT(r.status == SUCCESS, "pop 3 status");
    ASSERT(strcmp(r.value, "first") == 0, "expected 'first'");
    free(r.value);

    stack_destroy(&s);
    PASS();
}

static void test_pop_empty(void)
{
    TEST("pop on empty stack returns ERR_EMPTY");
    Stack *s = NULL;
    stack_create(4, &s);

    StringResult r = stack_pop(s);
    ASSERT(r.status == ERR_EMPTY, "expected ERR_EMPTY");
    ASSERT(r.value == NULL, "value should be NULL");

    stack_destroy(&s);
    PASS();
}

static void test_peek_returns_copy(void)
{
    TEST("peek returns a separate copy");
    Stack *s = NULL;
    stack_create(4, &s);
    stack_push(s, "hello");

    StringResult r1 = stack_peek(s);
    StringResult r2 = stack_peek(s);
    ASSERT(r1.status == SUCCESS && r2.status == SUCCESS,
           "both peeks should succeed");
    ASSERT(r1.value != r2.value, "two peeks should return different pointers");
    ASSERT(strcmp(r1.value, r2.value) == 0, "but same content");

    free(r1.value);
    free(r2.value);
    stack_destroy(&s);
    PASS();
}

static void test_peek_empty(void)
{
    TEST("peek on empty stack returns ERR_EMPTY");
    Stack *s = NULL;
    stack_create(4, &s);

    StringResult r = stack_peek(s);
    ASSERT(r.status == ERR_EMPTY, "expected ERR_EMPTY");
    ASSERT(r.value == NULL, "value should be NULL");

    stack_destroy(&s);
    PASS();
}

static void test_is_empty(void)
{
    TEST("is_empty reflects actual state");
    Stack *s = NULL;
    stack_create(4, &s);

    BoolResult b = stack_is_empty(s);
    ASSERT(b.status == SUCCESS && b.value == true, "new stack should be empty");

    stack_push(s, "x");
    b = stack_is_empty(s);
    ASSERT(b.status == SUCCESS && b.value == false,
           "stack with one element should not be empty");

    StringResult r = stack_pop(s);
    free(r.value);
    b = stack_is_empty(s);
    ASSERT(b.status == SUCCESS && b.value == true,
           "stack should be empty again after pop");

    stack_destroy(&s);
    PASS();
}

static void test_expansion(void)
{
    TEST("stack expands beyond initial capacity");
    Stack *s = NULL;
    stack_create(2, &s);

    for (int i = 0; i < 20; i++) {
        char buf[16];
        sprintf(buf, "item_%d", i);
        Status st = stack_push(s, buf);
        ASSERT(st == SUCCESS, "push during expansion");
    }

    SizeResult sz = stack_size(s);
    ASSERT(sz.value == 20, "expected size 20 after expansion");

    SizeResult cap = stack_capacity(s);
    ASSERT(cap.value >= 20, "capacity should be >= 20");

    for (int i = 19; i >= 0; i--) {
        char expected[16];
        sprintf(expected, "item_%d", i);
        StringResult r = stack_pop(s);
        ASSERT(r.status == SUCCESS, "pop status");
        ASSERT(strcmp(r.value, expected) == 0, "LIFO order after expand");
        free(r.value);
    }

    stack_destroy(&s);
    PASS();
}

static void test_null_stack_observers(void)
{
    TEST("observers reject NULL stack");
    ASSERT(stack_size(NULL).status == ERR_NULL_ARG, "size(NULL)");
    ASSERT(stack_capacity(NULL).status == ERR_NULL_ARG, "capacity(NULL)");
    ASSERT(stack_is_empty(NULL).status == ERR_NULL_ARG, "is_empty(NULL)");
    ASSERT(stack_pop(NULL).status == ERR_NULL_ARG, "pop(NULL)");
    ASSERT(stack_peek(NULL).status == ERR_NULL_ARG, "peek(NULL)");
    PASS();
}

static void test_status_messages(void)
{
    TEST("every status code has a non-NULL message");
    Status codes[] = {
        SUCCESS,
        ERR_NULL_ARG,
        ERR_ALLOC,
        ERR_EMPTY,
        ERR_CAPACITY_OVERFLOW,
        ERR_INVALID_CAPACITY,
        ERR_STRING_TOO_LONG,
        ERR_MAX_CAPACITY
    };
    size_t n = sizeof(codes) / sizeof(codes[0]);
    for (size_t i = 0; i < n; i++) {
        const char *msg = status_message(codes[i]);
        ASSERT(msg != NULL, "message should not be NULL");
        ASSERT(strlen(msg) > 0, "message should not be empty");
    }
    PASS();
}

static void test_push_empty_string(void)
{
    TEST("push and pop an empty string");
    Stack *s = NULL;
    stack_create(4, &s);

    ASSERT(stack_push(s, "") == SUCCESS, "push empty string");

    StringResult r = stack_pop(s);
    ASSERT(r.status == SUCCESS, "pop status");
    ASSERT(r.value != NULL, "value should not be NULL");
    ASSERT(strcmp(r.value, "") == 0, "value should be empty string");
    free(r.value);

    stack_destroy(&s);
    PASS();
}

int main(void)
{
    printf("\nTests:\n\n");

    test_create_and_destroy();
    test_create_default_capacity();
    test_create_null_out();
    test_create_excessive_capacity();
    test_destroy_null_pointer();
    test_destroy_null_stack();
    test_push_and_size();
    test_push_null_args();
    test_push_copies_input();
    test_pop_order();
    test_pop_empty();
    test_peek_returns_copy();
    test_peek_empty();
    test_is_empty();
    test_expansion();
    test_null_stack_observers();
    test_status_messages();
    test_push_empty_string();

    printf("\nResults: %d / %d passed\n\n", tests_passed, tests_run);

    return (tests_passed == tests_run) ? 0 : 1;
}
