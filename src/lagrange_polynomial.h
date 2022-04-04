#ifndef IMAGE_PROCESSOR_LAGRANGE_POLYNOMIAL_H
#define IMAGE_PROCESSOR_LAGRANGE_POLYNOMIAL_H

#include "pixel.h"

#include <iterator>
#include <vector>

// Интерполяция многочленом Лагранжа, считается наивно по определению inplace
// Сложность: O(n) на создание + O(n^2) на подсчёт функции в точке.
template <typename Coefficients>
class InplacePixelLagrangePolynomial {
public:
    InplacePixelLagrangePolynomial() : x_(), y_(), size_(0){};

    InplacePixelLagrangePolynomial(const std::vector<Coefficients>& x, const std::vector<Coefficients>& y) {
        std::copy(x.begin(), x.end(), std::back_inserter(x_));
        std::copy(y.begin(), y.end(), std::back_inserter(y_));
        size_ = x.size();
    }

    Pixel operator()(const Pixel& var) const {
        Pixel answer{};
        if (!size_) {
            return var;
        }
        for (size_t i = 0; i < size_; i++) {
            Pixel l_i(1);
            for (size_t j = 0; j < size_; j++) {
                if (i != j) {
                    l_i = l_i * (var - x_[j]) / (x_[i] - x_[j]);
                }
            }
            answer += (l_i * y_[i]);
        }
        return answer;
    }

protected:
    std::vector<Coefficients> x_;
    std::vector<Coefficients> y_;
    size_t size_;
};
#endif  // IMAGE_PROCESSOR_LAGRANGE_POLYNOMIAL_H
