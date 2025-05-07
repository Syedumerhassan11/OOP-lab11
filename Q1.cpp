#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>

// Custom exception for type mismatch
class BadTypeException : public std::exception {
    std::string message;
public:
    BadTypeException(const std::string& expected, const std::string& actual) {
        message = "BadTypeException: Expected type " + expected + ", but got type " + actual;
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Type-erased base class
class Placeholder {
public:
    virtual ~Placeholder() {}
    virtual const std::type_info& type() const = 0;
    virtual Placeholder* clone() const = 0;
};

// Template wrapper to store any type
template<typename T>
class Holder : public Placeholder {
    T value;
public:
    Holder(const T& val) : value(val) {}
    const std::type_info& type() const override { return typeid(T); }
    Placeholder* clone() const override { return new Holder<T>(value); }
    T& get() { return value; }
};

// Type-safe container
class TypeSafeContainer {
    Placeholder* data;

public:
    TypeSafeContainer() : data(nullptr) {}

    ~TypeSafeContainer() {
        delete data;
    }

    TypeSafeContainer(const TypeSafeContainer& other) {
        data = other.data ? other.data->clone() : nullptr;
    }

    TypeSafeContainer& operator=(const TypeSafeContainer& other) {
        if (this != &other) {
            delete data;
            data = other.data ? other.data->clone() : nullptr;
        }
        return *this;
    }

    template<typename T>
    void store(const T& value) {
        delete data;
        data = new Holder<T>(value);
    }

    template<typename T>
    T get() const {
        if (!data) {
            throw BadTypeException(typeid(T).name(), "empty container");
        }

        if (data->type() != typeid(T)) {
            throw BadTypeException(typeid(T).name(), data->type().name());
        }

        return static_cast<Holder<T>*>(data)->get();
    }

    void clear() {
        delete data;
        data = nullptr;
    }

    bool hasValue() const {
        return data != nullptr;
    }
};
int main() {
    TypeSafeContainer container;

    container.store<int>(100);

    try {
        std::cout << "Stored int: " << container.get<int>() << "\n";      // OK
        std::cout << "Trying to get double...\n";
        double d = container.get<double>();                                // Throws
    } catch (const BadTypeException& e) {
        std::cerr << e.what() << "\n";
    }

    return 0;
}

