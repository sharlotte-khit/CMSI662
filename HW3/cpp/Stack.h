#ifndef STACK_H
#define STACK_H

#include <cstddef>
#include <memory>
#include <string>

class Stack {
public:
    static constexpr size_t MAX_STRING_LENGTH = 1048576;
    static constexpr size_t MAX_CAPACITY = 1048576;
    static constexpr size_t DEFAULT_CAPACITY = 16;

    Stack(size_t initial_capacity = DEFAULT_CAPACITY);
    ~Stack();

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    Stack(Stack&& other) noexcept;
    Stack& operator=(Stack&& other) noexcept;

    void push(const std::string& str);
    std::string pop();
    std::string peek() const;

    size_t size() const noexcept;
    size_t capacity() const noexcept;
    bool is_empty() const noexcept;
    void clear() noexcept;

private:
    static constexpr size_t GROWTH_FACTOR = 2;

    std::unique_ptr<std::string>* data_;
    size_t size_;
    size_t capacity_;

    void expand();
    static void wipe(std::string& str) noexcept;
};

#endif
