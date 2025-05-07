#include <iostream>
#include <vector>
#include <stdexcept>
#include <sstream>

// Custom exception for dimension mismatch
class DimensionMismatchException : public std::exception {
    std::string message;
public:
    DimensionMismatchException(const std::string& msg) : message(msg) {}

    const char* what() const throw() override {
        return message.c_str();
    }
};

template<typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;
    size_t rows, cols;

public:
    // Constructor
    Matrix(size_t r, size_t c, const T& init = T()) : rows(r), cols(c) {
        data.assign(r, std::vector<T>(c, init));
    }

    // Get dimensions
    size_t numRows() const { return rows; }
    size_t numCols() const { return cols; }

    // Safe access with bounds checking
    T& at(size_t r, size_t c) {
        if (r >= rows || c >= cols)
            throw std::out_of_range("Matrix index out of range");
        return data[r][c];
    }

    const T& at(size_t r, size_t c) const {
        if (r >= rows || c >= cols)
            throw std::out_of_range("Matrix index out of range");
        return data[r][c];
    }

    // Operator+
    Matrix<T> operator+(const Matrix<T>& other) const {
        if (rows != other.rows || cols != other.cols)
            throw DimensionMismatchException("Matrix addition dimension mismatch");

        Matrix<T> result(rows, cols);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                result.at(i, j) = this->at(i, j) + other.at(i, j);

        return result;
    }

    // Operator*
    Matrix<T> operator*(const Matrix<T>& other) const {
        if (cols != other.rows)
            throw DimensionMismatchException("Matrix multiplication dimension mismatch");

        Matrix<T> result(rows, other.cols, T());
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                for (size_t k = 0; k < cols; ++k) {
                    result.at(i, j) += this->at(i, k) * other.at(k, j);
                }
            }
        }
        return result;
    }

    // Print matrix (for testing)
    void print() const {
        for (const auto& row : data) {
            for (const auto& val : row)
                std::cout << val << " ";
            std::cout << "\n";
        }
    }
};
int main() {
    try {
        Matrix<int> A(2, 2, 1); // 2x2 matrix filled with 1
        Matrix<int> B(2, 2, 2); // 2x2 matrix filled with 2

        Matrix<int> C = A + B;
        Matrix<int> D = A * B;

        std::cout << "A + B:\n";
        C.print();

        std::cout << "A * B:\n";
        D.print();

        std::cout << "Accessing element A.at(1, 1): " << A.at(1, 1) << "\n";

        // Uncomment to test out-of-bounds
        // std::cout << A.at(5, 5);

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

