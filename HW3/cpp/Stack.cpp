#include "Stack.h"

#include <stdexcept>
#include <utility>

void Stack::wipe(std::string& str) noexcept {
    volatile char* p = &str[0];
    for (size_t i = 0; i < str.size(); i++) {
        p[i] = '\0';
    }
    str.clear();
}

Stack::Stack(size_t initial_capacity)
    : data_(nullptr), size_(0), capacity_(0)
{
    if (initial_capacity == 0 || initial_capacity > MAX_CAPACITY) {
        throw std::invalid_argument(
            "Initial capacity must be in [1, "
            + std::to_string(MAX_CAPACITY) + "]; got "
            + std::to_string(initial_capacity));
    }
    data_ = new std::unique_ptr<std::string>[initial_capacity];
    capacity_ = initial_capacity;
}

Stack::~Stack() {
    clear();
    delete[] data_;
    data_ = nullptr;
    capacity_ = 0;
}

Stack::Stack(Stack&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
{
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

Stack& Stack::operator=(Stack&& other) noexcept {
    if (this != &other) {
        clear();
        delete[] data_;

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

void Stack::push(const std::string& str) {
    if (str.length() > MAX_STRING_LENGTH) {
        throw std::invalid_argument(
            "String length " + std::to_string(str.length())
            + " exceeds maximum of " + std::to_string(MAX_STRING_LENGTH));
    }

    if (size_ >= capacity_) {
        expand();
    }

    data_[size_] = std::make_unique<std::string>(str);
    size_++;
}

std::string Stack::pop() {
    if (size_ == 0) {
        throw std::underflow_error("Cannot pop from an empty stack");
    }

    size_--;
    std::string value = std::move(*data_[size_]);
    wipe(*data_[size_]);
    data_[size_].reset();
    return value;
}

std::string Stack::peek() const {
    if (size_ == 0) {
        throw std::underflow_error("Cannot peek at an empty stack");
    }
    return std::string(*data_[size_ - 1]);
}

size_t Stack::size() const noexcept {
    return size_;
}

size_t Stack::capacity() const noexcept {
    return capacity_;
}

bool Stack::is_empty() const noexcept {
    return size_ == 0;
}

void Stack::clear() noexcept {
    if (data_ == nullptr) {
        return;
    }
    for (size_t i = 0; i < size_; ++i) {
        if (data_[i]) {
            wipe(*data_[i]);
            data_[i].reset();
        }
    }
    size_ = 0;
}

void Stack::expand() {
    if (capacity_ >= MAX_CAPACITY) {
        throw std::length_error(
            "Stack has reached the maximum permitted capacity of "
            + std::to_string(MAX_CAPACITY));
    }

    size_t new_capacity = capacity_ * GROWTH_FACTOR;
    if (new_capacity > MAX_CAPACITY) {
        new_capacity = MAX_CAPACITY;
    }

    auto new_data = new std::unique_ptr<std::string>[new_capacity];
    for (size_t i = 0; i < size_; ++i) {
        new_data[i] = std::move(data_[i]);
    }

    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
}
