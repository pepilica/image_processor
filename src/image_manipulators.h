#ifndef IMAGE_PROCESSOR_IMAGE_MANIPULATORS_H
#define IMAGE_PROCESSOR_IMAGE_MANIPULATORS_H

#include "bitmap.h"
#include "lagrange_polynomial.h"
#include "matrix.h"
#include "pixel.h"
#include "poly.h"

#include <cmath>
#include <cstddef>
#include <iostream>

namespace ManipulatorParameters {
using ManipulatorBaseType = double;
const std::vector<std::vector<ManipulatorBaseType> > SharpeningFilterBase =
    std::vector<std::vector<ManipulatorBaseType> >({{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}});

const std::vector<std::vector<ManipulatorBaseType> > EdgeDetectionFilterBase =
    std::vector<std::vector<ManipulatorBaseType> >({{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}});
}  // namespace ManipulatorParameters

class Manipulator {
public:
    virtual void Apply(Matrix<Pixel>& data) const = 0;
    Manipulator() = default;
    Manipulator(const Manipulator& other) = delete;
    Manipulator& operator=(const Manipulator& other) = delete;
    virtual ~Manipulator() = default;
};

class ConvolutionalManipulator : public Manipulator {
protected:
    Matrix<ManipulatorParameters::ManipulatorBaseType> filter_;
};

class CustomManipulator : public Manipulator {};

class ToGreyscaleFilter : public CustomManipulator {
public:
    ToGreyscaleFilter();
    void Apply(Matrix<Pixel>& data) const override;
    static std::string GetHelp();
};

class ToGreyscaleBasicFilter : public CustomManipulator {
public:
    ToGreyscaleBasicFilter();
    void Apply(Matrix<Pixel>& data) const override;
    static std::string GetHelp();
};

class SharpeningFilter : public ConvolutionalManipulator {
public:
    SharpeningFilter();
    void Apply(Matrix<Pixel>& data) const override;
    static std::string GetHelp();
};

class EdgeDetectionFilter : public ConvolutionalManipulator {
public:
    explicit EdgeDetectionFilter(double threshold, Pixel black = {0, 0, 0}, Pixel white = {1, 1, 1});
    void Apply(Matrix<Pixel>& data) const override;
    static std::string GetHelp();

protected:
    double threshold_;
    Pixel black_;
    Pixel white_;
};

class NegativeFilter : public CustomManipulator {
public:
    NegativeFilter() = default;
    void Apply(Matrix<Pixel>& data) const override;
    static std::string GetHelp();
};

class GaussianBlurFilter : public CustomManipulator {
public:
    explicit GaussianBlurFilter(double sigma);
    void Apply(Matrix<Pixel>& data) const override;
    static std::string GetHelp();

protected:
    void VerticalGaussianBlur(Matrix<Pixel>& data) const;
    void HorizontalGaussianBlur(Matrix<Pixel>& data) const;
    double sigma_;
};

class FastGaussianBlurFilter : public CustomManipulator {
public:
    explicit FastGaussianBlurFilter(double sigma);
    void Apply(Matrix<Pixel>& data) const override;
    static std::string GetHelp();

protected:
    double sigma_;
    Matrix<PixelParameters::Scalar> vertical_convolution_;
    Matrix<PixelParameters::Scalar> horizontal_convolution_;
};

class CropFilter : public CustomManipulator {
public:
    explicit CropFilter(size_t width, size_t height);
    void Apply(Matrix<Pixel>& data) const override;
    static std::string GetHelp();

protected:
    size_t width_;
    size_t height_;
};

class CurvesFilter : public CustomManipulator {
public:
    explicit CurvesFilter(std::vector<ColourParameters::ColourType> x, std::vector<ColourParameters::ColourType> y);
    void Apply(Matrix<Pixel>& data) const override;
    static std::string GetHelp();

protected:
    PixelLagrangePolynomial<ColourParameters::ColourType> lagrange_poly_;
};

#endif  // IMAGE_PROCESSOR_IMAGE_MANIPULATORS_H
