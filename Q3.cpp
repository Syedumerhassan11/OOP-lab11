#include <iostream>
#include <stdexcept>
#include <string>

// Custom exception
class ArrayIndexOutOfBounds : public std::exception {
    std::string message;
public:
    ArrayIndexOutOfBounds(int index, int size) {
        message = "ArrayIndexOutOfBounds: index " + std::to_string(index) +
                  " is out of range (size: " + std::to_string(size) + ")";
    }

    const char* what() const throw() override {
        return message.c_str();
    }
};

// SafeArray template
template<typename T>
class SafeArray {
private:
    T* arr;
    int size;

public:
    SafeArray(int s) : size(s) {
        if (s <= 0)
            throw std::invalid_argument("Size must be positive");
        arr = new T[size];
    }

    ~SafeArray() {
        delete[] arr;
    }

    // Disable copy
    SafeArray(const SafeArray&) = delete;
    SafeArray& operator=(const SafeArray&) = delete;

    // Enable move (optional)
    SafeArray(SafeArray&& other) noexcept : arr(other.arr), size(other.size) {
        other.arr = nullptr;
        other.size = 0;
    }

    SafeArray& operator=(SafeArray&& other) noexcept {
        if (this != &other) {
            delete[] arr;
            arr = other.arr;
            size = other.size;
            other.arr = nullptr;
            other.size = 0;
        }
        return *this;
    }

    int getSize() const { return size; }

    // Overloaded operator[]
    T& operator[](int index) {
        if (index < 0 || index >= size)
            throw ArrayIndexOutOfBounds(index, size);
        return arr[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size)
            throw ArrayIndexOutOfBounds(index, size);
        return arr[index];
    }
};
int main() {
    try {
        SafeArray<int> arr(5);

        for (int i = 0; i < arr.getSize(); ++i)
            arr[i] = i * 10;

        std::cout << "SafeArray contents:\n";
        for (int i = 0; i < arr.getSize(); ++i)
            std::cout << arr[i] << " ";
        std::cout << "\n";

        std::cout << "Trying to access out-of-bounds index...\n";
        std::cout << arr[10]; // Will throw

    } catch (const ArrayIndexOutOfBounds& ex) {
        std::cerr << ex.what() << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "Other exception: " << ex.what() << "\n";
    }

    return 0;
}

