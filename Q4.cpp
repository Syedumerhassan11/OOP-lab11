#include <iostream>

// Templated Base class
template<typename T>
class Base {
protected:
    T a, b;
public:
    Base(T x, T y) : a(x), b(y) {}

    void multiply() const {
        std::cout << "Base multiplication (a * b): " << a * b << std::endl;
    }
};

// Templated Derived class inheriting from Base
template<typename T>
class Derived : public Base<T> {
private:
    T c, d;
public:
    Derived(T x, T y, T z, T w) : Base<T>(x, y), c(z), d(w) {}

    void multiplyAll() const {
        // multiply base class values
        T baseProduct = this->a * this->b;
        std::cout << "Base part (a * b): " << baseProduct << std::endl;

        // multiply derived class values
        T derivedProduct = c * d;
        std::cout << "Derived part (c * d): " << derivedProduct << std::endl;

        // full product
        T totalProduct = baseProduct * derivedProduct;
        std::cout << "Total (a*b*c*d): " << totalProduct << std::endl;
    }
};

// Demo program
int main() {
    Derived<int> obj(2, 3, 4, 5);  // a=2, b=3, c=4, d=5

    std::cout << "Calling base multiply function:\n";
    obj.multiply();  // from Base

    std::cout << "\nCalling derived multiplyAll function:\n";
    obj.multiplyAll();

    return 0;
}

