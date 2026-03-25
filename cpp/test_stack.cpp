#include "Stack.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

static int tests_run = 0;
static int tests_passed = 0;

static void pass(const std::string& name) {
    tests_passed++;
    std::cout << "  " << name << " ... PASS\n";
}

static void fail(const std::string& name, const std::string& msg) {
    std::cout << "  " << name << " ... FAIL (" << msg << ")\n";
}

static void test_default_construction() {
    tests_run++;
    Stack s;
    if (s.size() != 0) {
        fail("new stack starts empty", "size != 0");
        return;
    }
    if (!s.is_empty()) {
        fail("new stack starts empty", "not empty");
        return;
    }
    pass("new stack starts empty");
}

static void test_explicit_capacity() {
    tests_run++;
    Stack s(4);
    if (s.capacity() != 4) {
        fail("constructor uses given capacity", "wrong capacity");
        return;
    }
    if (s.size() != 0) {
        fail("constructor uses given capacity", "size != 0");
        return;
    }
    pass("constructor uses given capacity");
}

static void test_zero_capacity_throws() {
    tests_run++;
    try {
        Stack s(0);
        fail("capacity 0 throws exception", "no exception");
        return;
    } catch (const std::invalid_argument&) {}
    pass("capacity 0 throws exception");
}

static void test_excessive_capacity_throws() {
    tests_run++;
    try {
        Stack s(Stack::MAX_CAPACITY + 1);
        fail("capacity over max throws exception", "no exception");
        return;
    } catch (const std::invalid_argument&) {}
    pass("capacity over max throws exception");
}

static void test_push_and_pop() {
    tests_run++;
    Stack s(2);
    s.push("alpha");
    s.push("beta");
    s.push("gamma");

    if (s.size() != 3) {
        fail("pop returns elements in LIFO order", "size != 3");
        return;
    }
    if (s.pop() != "gamma") {
        fail("pop returns elements in LIFO order", "expected gamma");
        return;
    }
    if (s.pop() != "beta") {
        fail("pop returns elements in LIFO order", "expected beta");
        return;
    }
    if (s.pop() != "alpha") {
        fail("pop returns elements in LIFO order", "expected alpha");
        return;
    }
    if (!s.is_empty()) {
        fail("pop returns elements in LIFO order", "not empty");
        return;
    }
    pass("pop returns elements in LIFO order");
}

static void test_peek() {
    tests_run++;
    Stack s;
    s.push("only");
    if (s.peek() != "only") {
        fail("peek shows top without removing it", "wrong value");
        return;
    }
    if (s.size() != 1) {
        fail("peek shows top without removing it", "size changed");
        return;
    }
    pass("peek shows top without removing it");
}

static void test_pop_empty_throws() {
    tests_run++;
    Stack s;
    try {
        s.pop();
        fail("pop on empty stack throws exception", "no exception");
        return;
    } catch (const std::underflow_error&) {}
    pass("pop on empty stack throws exception");
}

static void test_peek_empty_throws() {
    tests_run++;
    Stack s;
    try {
        s.peek();
        fail("peek on empty stack throws exception", "no exception");
        return;
    } catch (const std::underflow_error&) {}
    pass("peek on empty stack throws exception");
}

static void test_push_copies_input() {
    tests_run++;
    Stack s;
    std::string original = "secret";
    s.push(original);
    original = "CHANGED";
    if (s.peek() != "secret") {
        fail("push stores a copy of the string", "data changed");
        return;
    }
    pass("push stores a copy of the string");
}

static void test_peek_returns_copy() {
    tests_run++;
    Stack s;
    s.push("data");
    std::string a = s.peek();
    std::string b = s.peek();
    if (&a == &b) {
        fail("peek returns a new copy each call", "same object");
        return;
    }
    if (a != b) {
        fail("peek returns a new copy each call", "different content");
        return;
    }
    pass("peek returns a new copy each call");
}

static void test_clear() {
    tests_run++;
    Stack s;
    s.push("a");
    s.push("b");
    s.clear();
    if (!s.is_empty()) {
        fail("clear removes all elements", "not empty");
        return;
    }
    if (s.size() != 0) {
        fail("clear removes all elements", "size != 0");
        return;
    }
    pass("clear removes all elements");
}

static void test_expansion() {
    tests_run++;
    Stack s(2);
    for (int i = 0; i < 20; i++) {
        s.push("item_" + std::to_string(i));
    }
    if (s.size() != 20) {
        fail("stack grows when full", "size != 20");
        return;
    }
    if (s.capacity() < 20) {
        fail("stack grows when full", "capacity < 20");
        return;
    }
    for (int i = 19; i >= 0; i--) {
        std::string expected = "item_" + std::to_string(i);
        if (s.pop() != expected) {
            fail("stack grows when full", "wrong LIFO order");
            return;
        }
    }
    pass("stack grows when full");
}

static void test_move_constructor() {
    tests_run++;
    Stack a(4);
    a.push("moved");
    Stack b(std::move(a));

    if (b.size() != 1) {
        fail("move constructor transfers data", "b size != 1");
        return;
    }
    if (b.pop() != "moved") {
        fail("move constructor transfers data", "wrong value");
        return;
    }
    if (a.size() != 0) {
        fail("move constructor transfers data", "a not empty");
        return;
    }
    pass("move constructor transfers data");
}

static void test_move_assignment() {
    tests_run++;
    Stack a(4);
    a.push("data");
    Stack b(2);
    b = std::move(a);

    if (b.size() != 1) {
        fail("move assignment transfers data", "b size != 1");
        return;
    }
    if (b.pop() != "data") {
        fail("move assignment transfers data", "wrong value");
        return;
    }
    pass("move assignment transfers data");
}

static void test_push_empty_string() {
    tests_run++;
    Stack s;
    s.push("");
    if (s.pop() != "") {
        fail("push and pop empty string works", "wrong value");
        return;
    }
    pass("push and pop empty string works");
}

int main() {
    std::cout << "\nTests:\n\n";

    test_default_construction();
    test_explicit_capacity();
    test_zero_capacity_throws();
    test_excessive_capacity_throws();
    test_push_and_pop();
    test_peek();
    test_pop_empty_throws();
    test_peek_empty_throws();
    test_push_copies_input();
    test_peek_returns_copy();
    test_clear();
    test_expansion();
    test_move_constructor();
    test_move_assignment();
    test_push_empty_string();

    std::cout << "\nResults: " << tests_passed << " / " << tests_run << " passed\n\n";

    return (tests_passed == tests_run) ? 0 : 1;
}
