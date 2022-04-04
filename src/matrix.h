#ifndef IMAGE_PROCESSOR_MATRIX_H
#define IMAGE_PROCESSOR_MATRIX_H

#include <iostream>
#include <stdexcept>
#include <vector>

namespace MatrixParameters {
class SizesNotSuitableException : public std::exception {
    const char* what() const throw() {
        return "Sizes of matrices are not suitable";
    }
};
using StandardElementType = int64_t;
}  // namespace MatrixParameters

template <typename ElementType>
class Matrix {
public:
    using MatrixCore = std::vector<ElementType>;
    using ValuesTable = std::vector<std::vector<ElementType> >;

    Matrix() : matrix_(nullptr), height_(0), width_(0){};

    Matrix(const Matrix& other) : matrix_(nullptr), height_(0), width_(0) {
        Copy(other);
    }

    Matrix(Matrix&& other) : matrix_(nullptr), height_(0), width_(0) {
        std::swap(matrix_, other.matrix_);
        std::swap(width_, other.width_);
        std::swap(height_, other.height_);
    }

    explicit Matrix(const ValuesTable& other) : matrix_(nullptr), height_(0), width_(0) {
        size_t column_size = other.size();
        height_ = column_size;
        if (column_size > 0) {
            size_t row_size = other[0].size();
            width_ = row_size;
            matrix_ = new MatrixCore(row_size * column_size);
            auto starting_point = matrix_->begin();
            for (size_t i = 0; i < column_size; ++i) {
                std::copy(other[i].begin(), other[i].end(), starting_point);
                std::advance(starting_point, row_size);
            }
        } else {
            height_ = 0;
            width_ = 0;
            matrix_ = nullptr;
        }
    }

    explicit Matrix(size_t width, size_t height) : matrix_(nullptr), height_(height), width_(width) {
        if (height == 0 || width == 0) {
            height_ = 0;
            width_ = 0;
            return;
        }
        matrix_ = new MatrixCore(height * width);
    }

    std::pair<size_t, size_t> GetSize() const {
        return {height_, width_};
    }
    size_t GetHeight() const {
        return height_;
    }
    size_t GetWidth() const {
        return width_;
    }

    ElementType& GetElement(size_t x, size_t y) {
        if (x >= width_) {
            throw std::out_of_range("X is out of bounds");
        }
        if (y >= height_) {
            throw std::out_of_range("Y is out of range");
        }
        return (*matrix_)[GetWidth() * y + x];
    }
    ElementType GetElement(size_t x, size_t y) const {
        return (*matrix_)[GetWidth() * y + x];
    }

    template <typename T>
    Matrix<ElementType>& Convolution(const Matrix<T>& other) {
        size_t other_height = other.GetHeight();
        size_t other_width = other.GetWidth();
        size_t mid_height = (other_height) / 2;
        size_t mid_width = (other_width) / 2;
        Matrix temp_answer = *this;
        for (size_t i = 0; i < GetHeight(); ++i) {
            for (size_t j = 0; j < GetWidth(); ++j) {
                ElementType temp_answer_xy{};
                for (size_t y_ij = 0; y_ij < other_height; ++y_ij) {
                    for (size_t x_ij = 0; x_ij < other_width; ++x_ij) {
                        size_t index_x_ij{};
                        size_t index_y_ij{};
                        if (mid_width > x_ij + j) {
                            index_x_ij = 0;
                        } else {
                            index_x_ij = std::min(x_ij + j - mid_width, GetWidth() - 1);
                        }
                        if (mid_height > y_ij + i) {
                            index_y_ij = 0;
                        } else {
                            index_y_ij = std::min(y_ij + i - mid_height, GetHeight() - 1);
                        }
                        const ElementType& closest_to_ij = GetElement(index_x_ij, index_y_ij);
                        const T& other_element_ij = other.GetElement(x_ij, y_ij);
                        temp_answer_xy += (closest_to_ij * other_element_ij);
                    }
                }
                temp_answer.GetElement(j, i) = temp_answer_xy;
            }
        }
        Copy(temp_answer);
        return *this;
    }

    Matrix& operator=(const Matrix& other) {
        if (this == &other) {
            return *this;
        }
        matrix_ = new MatrixCore(other.height_ * other.width_);
        height_ = other.GetHeight();
        width_ = other.GetWidth();
        std::copy(other.matrix_->begin(), other.matrix_->end(), matrix_->begin());
        return *this;
    }

    Matrix& operator=(Matrix&& other) {
        if (this == &other) {
            return *this;
        }
        std::swap(other.matrix_, matrix_);
        std::swap(other.height_, height_);
        std::swap(other.width_, width_);
        return *this;
    }

    bool operator==(const Matrix& other) const {
        return (*other.matrix_ == *matrix_ && other.height_ == height_ && other.width_ == width_);
    }

    bool operator!=(const Matrix& other) const {
        return (*other.matrix_ != *matrix_ || other.height_ != height_ || other.width_ != width_);
    }

    ElementType operator*(const Matrix& other) const {
        ElementType answer = 0;
        if (GetSize() != other.GetSize()) {
            throw MatrixParameters::SizesNotSuitableException();
        }
        for (size_t i = 0; i < GetWidth() * GetHeight(); ++i) {
            answer += (*matrix_)[i] * (*other.matrix_)[i];
        }
        return answer;
    }

    void Resize(size_t new_width, size_t new_height) {
        const size_t width = new_width + 0;
        const size_t height = new_height + 0;
        Matrix<ElementType> temp(width, height);
        size_t min_width = std::min(width_, new_width);
        size_t min_height = std::min(height_, new_height);
        for (size_t y = 0; y < min_height; ++y) {
            for (size_t x = 0; x < min_width; ++x) {
                ElementType& temp_xy = temp.GetElement(x, y);
                temp_xy = this->GetElement(x, y);
            }
        }
        Copy(temp);
    }

    ~Matrix() {
        if (matrix_ != nullptr) {
            delete matrix_;
        }
    }

private:
    MatrixCore* matrix_;
    size_t height_;
    size_t width_;

    void Copy(const Matrix& other) {
        MatrixCore* temp = new MatrixCore(other.height_ * other.width_);
        delete matrix_;
        matrix_ = nullptr;
        height_ = other.height_;
        width_ = other.width_;
        std::copy(other.matrix_->begin(), other.matrix_->end(), temp->begin());
        std::swap(matrix_, temp);
    }
};

#endif  // IMAGE_PROCESSOR_MATRIX_H
