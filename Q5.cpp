#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

// Custom exception class
class StackUnderflowException : public std::exception {
    std::string message;
public:
    StackUnderflowException(const std::string& msg = "Stack underflow: operation on empty stack") {
        message = msg;
    }

    const char* what() const throw() override {
        return message.c_str();
    }
};

// Stack class template
template<typename T>
class Stack {
private:
    std::vector<T> data;

public:
    // Push an element
    void push(const T& value) {
        data.push_back(value);
    }

    // Pop the top element
    void pop() {
        if (data.empty())
            throw StackUnderflowException("Cannot pop from empty stack.");
        data.pop_back();
    }

    // Access the top element
    T& top() {
        if (data.empty())
            throw StackUnderflowException("Cannot access top of empty stack.");
        return data.back();
    }

    // Check if the stack is empty
    bool isEmpty() const {
        return data.empty();
    }

    // Get stack size
    size_t size() const {
        return data.size();
    }
};
int main() {
    Stack<int> s;

    try {
        std::cout << "Pushing 10, 20, 30 onto the stack...\n";
        s.push(10);
        s.push(20);
        s.push(30);

        std::cout << "Top element: " << s.top() << "\n";
        s.pop();
        std::cout << "Top after pop: " << s.top() << "\n";

        std::cout << "Popping all elements...\n";
        s.pop();
        s.pop();

        std::cout << "Attempting to pop from empty stack...\n";
        s.pop();  // Should throw

    } catch (const StackUnderflowException& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    try {
        std::cout << "Attempting to access top of empty stack...\n";
        std::cout << s.top();  // Should throw
    } catch (const StackUnderflowException& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

