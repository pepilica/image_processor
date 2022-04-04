#include "image_manipulators.h"

ToGreyscaleFilter::ToGreyscaleFilter() = default;

SharpeningFilter::SharpeningFilter() {
    filter_ = Matrix<ManipulatorParameters::ManipulatorBaseType>(ManipulatorParameters::SharpeningFilterBase);
}

void SharpeningFilter::Apply(Matrix<Pixel>& data) const {
    data.Convolution(filter_);
}

std::string SharpeningFilter::GetHelp() {
    return "Sharpening Filter (-sharp):\n"
           "Makes the picture sharper.\n\n"
           "Parameters: none";
}

void ToGreyscaleFilter::Apply(Matrix<Pixel>& data) const {
    for (size_t y = 0; y < data.GetHeight(); ++y) {
        for (size_t x = 0; x < data.GetWidth(); ++x) {
            Pixel& temp = data.GetElement(x, y);
            temp = Pixel(0.299 * temp.GetRed() + 0.587 * temp.GetGreen() + 0.114 * temp.GetBlue());
        }
    }
}

std::string ToGreyscaleFilter::GetHelp() {
    return "To Greyscale Filter (-gs):\n"
           "Converts the picture to the greyscale using the formula R' = G' = B' = 0.299 * R + 0.587 * G + 0.114 * B\n"
           "Parameters: none";
}

EdgeDetectionFilter::EdgeDetectionFilter(double threshold, Pixel black, Pixel white) {
    filter_ = Matrix<ManipulatorParameters::ManipulatorBaseType>(ManipulatorParameters::EdgeDetectionFilterBase);
    threshold_ = threshold;
    black_ = black;
    white_ = white;
}

void EdgeDetectionFilter::Apply(Matrix<Pixel>& data) const {

    for (size_t y = 0; y < data.GetHeight(); ++y) {
        for (size_t x = 0; x < data.GetWidth(); ++x) {
            data.GetElement(x, y).ToGreyscale();
        }
    }
    data.Convolution(filter_);
    for (size_t y = 0; y < data.GetHeight(); ++y) {
        for (size_t x = 0; x < data.GetWidth(); ++x) {
            Pixel& pixel_xy = data.GetElement(x, y);
            if (pixel_xy.GetRed() >= threshold_) {
                pixel_xy = white_;
            } else {
                pixel_xy = black_;
            }
        }
    }
}

std::string EdgeDetectionFilter::GetHelp() {
    return "Edge Detection Filter (-edge threshold):\n"
           "Converts the picture to greyscale using naive formula, then applies the detection using convolution."
           "If the pixel is brighter than threshold, it is painted white, otherwise is painted black.\n"
           "Parameters: double threshold - [0, 1]";
}

ToGreyscaleBasicFilter::ToGreyscaleBasicFilter() = default;

void ToGreyscaleBasicFilter::Apply(Matrix<Pixel>& data) const {
    for (size_t y = 0; y < data.GetHeight(); ++y) {
        for (size_t x = 0; x < data.GetWidth(); ++x) {
            data.GetElement(x, y).ToGreyscale();
        }
    }
}

std::string ToGreyscaleBasicFilter::GetHelp() {
    return "To Greyscale Filter Basic (-gsbasic):\n"
           "Converts the picture to the greyscale using naive formula\n"
           "Parameters: none";
}

void NegativeFilter::Apply(Matrix<Pixel>& data) const {
    for (size_t y = 0; y < data.GetHeight(); ++y) {
        for (size_t x = 0; x < data.GetWidth(); ++x) {
            Pixel& temp = data.GetElement(x, y);
            temp.ToNegative();
        }
    }
}

std::string NegativeFilter::GetHelp() {
    return "Negative Filter (-sharp):\n"
           "Makes the picture negative.\n\n"
           "Parameters: none";
}

void GaussianBlurFilter::HorizontalGaussianBlur(Matrix<Pixel>& data) const {

    size_t width = data.GetWidth();
    size_t height = data.GetHeight();
    Matrix<Pixel> temp(width, height);
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            Pixel temp_xy(0);
            for (size_t x_i = 0; x_i < width; ++x_i) {
                int64_t modulo_x = pow((std::max(x_i, x) - std::min(x_i, x)), 2);
                temp_xy += data.GetElement(x_i, y) * ((1 / (sqrt(2 * M_PI) * sigma_)) *
                                                      exp(-static_cast<double>(modulo_x) / (2 * sigma_ * sigma_)));
            }
            temp.GetElement(x, y) = temp_xy;
        }
    }
    data = temp;
}

void GaussianBlurFilter::VerticalGaussianBlur(Matrix<Pixel>& data) const {

    size_t width = data.GetWidth();
    size_t height = data.GetHeight();
    Matrix<Pixel> temp(width, height);
    std::cout << temp.GetHeight() << temp.GetWidth() << std::endl;
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            Pixel temp_xy(0);
            for (size_t y_i = 0; y_i < height; ++y_i) {
                int64_t modulo_y = pow((std::max(y_i, y) - std::min(y_i, y)), 2);
                temp_xy += data.GetElement(x, y_i) * ((1 / (sqrt(2 * M_PI) * sigma_)) *
                                                      exp(-static_cast<double>(modulo_y) / (2 * sigma_ * sigma_)));
            }
            temp.GetElement(x, y) = temp_xy;
        }
    }
    data = temp;
}

void GaussianBlurFilter::Apply(Matrix<Pixel>& data) const {
    VerticalGaussianBlur(data);
    HorizontalGaussianBlur(data);
}

std::string GaussianBlurFilter::GetHelp() {
    return "Gaussian Blur Filter (unspecified):\n"
           "Implementing the Gaussian blur using 2D full-width/height kernel\n"
           "Parameters: double sigma - [0, +inf)";
}
GaussianBlurFilter::GaussianBlurFilter(double sigma) : sigma_(sigma) {
}

FastGaussianBlurFilter::FastGaussianBlurFilter(double sigma) : sigma_(sigma) {
    size_t sizes = ceil(6 * sigma);
    if (sizes % 2 == 0) {
        ++sizes;
    }
    size_t mid = sizes / 2;
    vertical_convolution_ = Matrix<PixelParameters::Scalar>(1, sizes);
    horizontal_convolution_ = Matrix<PixelParameters::Scalar>(sizes, 1);
    for (size_t i = 0; i < sizes; ++i) {
        PixelParameters::Scalar elem_i;
        int64_t modulo_i = pow((std::max(i, mid) - std::min(i, mid)), 2);
        elem_i = ((1 / sqrt(2 * M_PI * sigma * sigma)) * exp(-static_cast<double>(modulo_i) / (2 * sigma_ * sigma_)));
        vertical_convolution_.GetElement(0, i) = elem_i;
        horizontal_convolution_.GetElement(i, 0) = elem_i;
    }
}

void FastGaussianBlurFilter::Apply(Matrix<Pixel>& data) const {

    data.Convolution(vertical_convolution_);
    data.Convolution(horizontal_convolution_);
}

std::string FastGaussianBlurFilter::GetHelp() {
    return "Fast Gaussian Blur Filter (-blur):\n"
           "Implementing the Gaussian blur using 2D shortened kernel\n"
           "Parameters: double sigma - [0, +inf)\n"
           "(p.s. not recommended for precise calculations, use Gaussian Blur Filter instead)";
}

CropFilter::CropFilter(size_t width, size_t height) : width_(width), height_(height) {
}

void CropFilter::Apply(Matrix<Pixel>& data) const {
    if (width_ < data.GetWidth() || height_ < data.GetHeight()) {
        data.Resize(std::min(data.GetWidth(), width_), (data.GetHeight(), height_));
    }
}

std::string CropFilter::GetHelp() {
    return "Crop Filter (-crop):\n"
           "Crops the picture with the left upper end at (0, 0), right lower end at (width, height), "
           "fills with black if the borders of the picture expanded. \n"
           "Parameters: width, height > 0 - positive values.\n";
}

CurvesFilter::CurvesFilter(std::vector<ColourParameters::ColourType> x, std::vector<ColourParameters::ColourType> y)
    : lagrange_poly_(x, y){};

void CurvesFilter::Apply(Matrix<Pixel>& data) const {
    for (size_t y = 0; y < data.GetHeight(); ++y) {
        for (size_t x = 0; x < data.GetWidth(); ++x) {
            Pixel& elem_xy = data.GetElement(x, y);
            elem_xy = lagrange_poly_(elem_xy);
        }
    }
}

std::string CurvesFilter::GetHelp() {
    return "Curves Filter (-curves):\n"
           "Makes the \"Curves\" transformation from Photoshop using Lagrangian polynomial."
           "Parameters: 2n points of type (x_i, y_i) - [0, 1].\n"
           "P.S. Throws exception if identical x-coordinates are found or the number of arguments is odd.";
}