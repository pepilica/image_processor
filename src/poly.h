#pragma once

#include <cmath>
#include <iostream>
#include <map>
#include <vector>

template <typename NumType>
class Poly {
public:
    Poly(const std::vector<NumType>& coefficients) {
        poly_coefficients_ = new std::map<size_t, NumType>;
        for (size_t i = 0; i < coefficients.size(); ++i) {
            const NumType& coefficient_i = coefficients[i];
            if (coefficient_i != 0) {
                poly_coefficients_->insert({i, coefficient_i});
            }
        }
    }

    Poly(const std::vector<std::pair<size_t, NumType>>& coefficients) {
        poly_coefficients_ = new std::map<size_t, NumType>;
        for (std::pair<size_t, NumType> i : coefficients) {
            if (i.second != 0) {
                poly_coefficients_->insert(i);
            }
        }
    }

    Poly() {
        poly_coefficients_ = new std::map<size_t, NumType>;
        poly_coefficients_->insert({0, 0});
    };

    Poly(const Poly& rhv) {
        poly_coefficients_ = new std::map<size_t, NumType>;
        std::map<size_t, NumType>& lhv_coef = *(poly_coefficients_);
        const std::map<size_t, NumType>& rhv_coef = *(rhv.poly_coefficients_);
        for (auto it = rhv_coef.begin(); it != rhv_coef.end(); ++it) {
            lhv_coef[it->first] = it->second;
        }
    }
    Poly(Poly&& rhv) = default;

    ~Poly() {
        delete poly_coefficients_;
    }

    Poly operator+(const Poly& rhv) const {
        Poly answer;
        answer.WeightedAddition(*this, 1);
        answer.WeightedAddition(rhv, 1);
        return answer;
    }

    Poly operator-(const Poly& rhv) const {
        Poly answer;
        answer.WeightedAddition(*this, 1);
        answer.WeightedAddition(rhv, -1);
        return answer;
    }

    template <typename Var>
    static Var Pow(const Var& input, size_t pow) {
        if (pow == 0) {
            return Var{1};
        }
        if (pow == 1) {
            return input;
        } else {
            if (pow % 2 == 0) {
                Var root_input = Pow(input, pow / 2);
                return root_input * root_input;
            } else {
                return input * Pow(input, pow - 1);
            }
        }
    }

    void operator+=(const Poly& rhv) {
        WeightedAddition(rhv, 1);
    }

    void operator-=(const Poly& rhv) {
        WeightedAddition(rhv, -1);
    }

    bool operator==(const Poly& rhv) const {
        return IsEqualTo(rhv);
    }

    bool operator!=(const Poly& rhv) const {
        return !IsEqualTo(rhv);
    }

    template <typename Variable>
    Variable operator()(const Variable& x) const {
        Variable answer{};
        const std::map<size_t, NumType>& coefficients = *poly_coefficients_;
        auto iter = coefficients.rbegin();
        size_t degree = coefficients.rbegin()->first;
        for (size_t it = 0; it <= degree; ++it) {
            answer = answer * x;
            if (iter != coefficients.rend()) {
                if (iter->first == degree - it) {
                    answer += iter->second;
                    ++iter;
                }
            }
        }
        return answer;
    };

    void operator=(const Poly& rhv) {
        delete poly_coefficients_;
        poly_coefficients_ = nullptr;
        poly_coefficients_ = new std::map<size_t, NumType>;
        std::map<size_t, NumType>& lhv_coef = *(poly_coefficients_);
        const std::map<size_t, NumType>& rhv_coef = *(rhv.poly_coefficients_);
        for (auto it = rhv_coef.begin(); it != rhv_coef.end(); ++it) {
            lhv_coef[it->first] = it->second;
        }
    }

    Poly& operator=(Poly&& rhv) = default;

    Poly operator-() const {
        Poly answer;
        answer.WeightedAddition(*this, -1);
        return answer;
    }

    Poly operator*(const Poly& rhv) const {
        Poly answer;
        std::map<size_t, NumType>& ans_coef = *(answer.poly_coefficients_);
        const std::map<size_t, NumType>& lhv_coef = *poly_coefficients_;
        const std::map<size_t, NumType>& rhv_coef = *(rhv.poly_coefficients_);

        for (auto it_i = lhv_coef.begin(); it_i != lhv_coef.end(); ++it_i) {
            for (auto it_j = rhv_coef.begin(); it_j != rhv_coef.end(); ++it_j) {
                ans_coef[it_i->first + it_j->first] += it_i->second * it_j->second;
            }
        }
        return answer;
    }

    Poly operator*(NumType scalar) const {
        Poly<NumType> answer;
        for (auto it_i = poly_coefficients_->begin(); it_i != poly_coefficients_->end(); ++it_i) {
            auto [coef_i, val_i] = *it_i;
            answer.poly_coefficients_->insert({coef_i, val_i * scalar});
        }
        return answer;
    }

    Poly& operator*=(NumType scalar) const {
        for (auto it_i = poly_coefficients_->begin(); it_i != poly_coefficients_->end(); ++it_i) {
            NumType& val_i = *it_i->second;
            val_i *= scalar;
        }
        return *this;
    }

    std::map<size_t, NumType>* GetCoefficientsTable() {
        return poly_coefficients_;
    }

protected:
    std::map<size_t, NumType>* poly_coefficients_ = nullptr;

    bool IsEqualTo(const Poly& rhv) const {
        const std::map<size_t, NumType>& rhv_coef = *(rhv.poly_coefficients_);
        std::map<size_t, NumType>& lhv_coef = *(poly_coefficients_);
        for (auto it = rhv_coef.begin(); it != rhv_coef.end(); ++it) {
            if (lhv_coef[it->first] != it->second) {
                return false;
            }
        }
        return true;
    }

    void WeightedAddition(const Poly& rhv, NumType weight) {
        const std::map<size_t, NumType>& rhv_coef = *(rhv.poly_coefficients_);
        std::map<size_t, NumType>& lhv_coef = *(poly_coefficients_);
        for (auto it = rhv_coef.begin(); it != rhv_coef.end(); ++it) {
            lhv_coef[it->first] += weight * it->second;
            if (lhv_coef[it->first] == 0) {
                auto needed_pos = lhv_coef.find(it->first);
                if (needed_pos != lhv_coef.end()) {
                    lhv_coef.erase(needed_pos);
                }
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& stream, const Poly& rhv) {
        const std::map<size_t, NumType>& coefficients = *(rhv.poly_coefficients_);
        stream << "y = ";
        if (coefficients.empty()) {
            stream << "0";
        } else {
            for (auto it = coefficients.rbegin(); it != coefficients.rend(); ++it) {
                if (it == coefficients.rbegin()) {
                    stream << it->second;
                } else {
                    stream << " ";
                    if (it->second >= 0) {
                        stream << "+ ";
                    } else {
                        stream << "- ";
                    }
                    stream << std::abs(it->second);
                }
                if (it->first > 0) {
                    stream << "x";
                    if (it->first > 1) {
                        stream << "^" << it->first;
                    }
                }
            }
        }
        return stream;
    }
};

// Интерполяция многочленом Лагранжа: делается предподсчёт многочлена Лагранжа, считает значение функции.
// Сложность: O(n^3) на конструкцию, O(n*g(n)) на подсчёт функции в точке (g(n) - на умножение, в нынешней реализации -
// - O(1)).
template <typename Coefficients>
class PixelLagrangePolynomial : public Poly<Coefficients> {
public:
    PixelLagrangePolynomial() : Poly<Coefficients>(){};

    explicit PixelLagrangePolynomial(const std::vector<Coefficients>& x, const std::vector<Coefficients>& y)
        : Poly<Coefficients>({0, 1}) {
        size_t size = x.size();
        if (size > 0) {
            Poly<Coefficients> temp({1});
            for (size_t i = 0; i < size; i++) {
                Poly<Coefficients> l_i({1});
                for (size_t j = 0; j < size; j++) {
                    if (i != j) {
                        Coefficients denominator = x[i] - x[j];
                        l_i = l_i * Poly<Coefficients>({-x[j] / denominator, 1 / denominator});
                    }
                }
                temp += (l_i * y[i]);
            }
            temp -= Poly<Coefficients>({1});
            *this->GetCoefficientsTable() = std::move(*temp.GetCoefficientsTable());
        }
    }
};