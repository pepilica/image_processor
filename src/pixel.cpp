#include <cmath>
#include "pixel.h"

Pixel::Pixel() {
    scheme_ = ColourParameters::ColourSchemes::RGB;
    red_ = 0;
    green_ = 0;
    blue_ = 0;
}

Pixel::Pixel(ColourParameters::ColourType value) {
    scheme_ = ColourParameters::ColourSchemes::Greyscale;
    ColourParameters::ColourType norm_value =
        std::clamp(value, static_cast<ColourParameters::ColourType>(0), static_cast<ColourParameters::ColourType>(1));
    red_ = norm_value;
    green_ = norm_value;
    blue_ = norm_value;
}

std::string Pixel::info() const {
    std::string info_temp;
    info_temp += "Red: " + std::to_string(red_) + "\n";
    info_temp += "Green: " + std::to_string(green_) + "\n";
    info_temp += "Blue: " + std::to_string(blue_) + "\n";
    return info_temp;
}

Pixel Pixel::InGrayScalePixel() const {
    double average = (red_ + green_ + blue_) / 3;
    return Pixel(average);
}

Pixel Pixel::InRgbPixel() const {
    return {red_, green_, blue_};
}

Pixel &Pixel::operator=(const Pixel &other) {
    scheme_ = other.scheme_;
    SetRed(other.GetRed());
    SetGreen(other.GetGreen());
    SetBlue(other.GetBlue());
    return *this;
}

Pixel Pixel::operator+(const Pixel &other) const {
    Pixel temp;
    if (scheme_ == ColourParameters::ColourSchemes::Greyscale &&
        other.scheme_ == ColourParameters::ColourSchemes::Greyscale) {
        temp.scheme_ = ColourParameters::ColourSchemes::Greyscale;
    } else {
        temp.scheme_ = ColourParameters::ColourSchemes::RGB;
    }
    temp.SetRed(GetRed() + other.GetRed());
    temp.SetGreen(GetGreen() + other.GetGreen());
    temp.SetBlue(GetBlue() + other.GetBlue());
    return temp;
}

Pixel &Pixel::operator+=(const Pixel &other) {
    if (other.scheme_ == ColourParameters::ColourSchemes::RGB &&
        scheme_ == ColourParameters::ColourSchemes::Greyscale) {
        scheme_ = ColourParameters::ColourSchemes::RGB;
    }
    SetRed(GetRed() + other.GetRed());
    SetGreen(GetGreen() + other.GetGreen());
    SetBlue(GetBlue() + other.GetBlue());
    return *this;
}

Pixel Pixel::operator*(PixelParameters::Scalar l) const {
    Pixel temp;
    temp.scheme_ = scheme_;
    temp.SetRed(GetRed() * l);
    temp.SetGreen(GetGreen() * l);
    temp.SetBlue(GetBlue() * l);
    return temp;
}

Pixel &Pixel::operator*=(PixelParameters::Scalar l) {
    SetRed(GetRed() * l);
    SetGreen(GetGreen() * l);
    SetBlue(GetBlue() * l);
    return *this;
}

ColourParameters::ColourSchemes Pixel::GetScheme() const {
    return scheme_;
}

Pixel &Pixel::ToGreyscale() {
    double average = (red_ + green_ + blue_) / 3;
    scheme_ = ColourParameters::ColourSchemes::Greyscale;
    SetRed(average);
    SetGreen(average);
    SetBlue(average);
    return *this;
}

Pixel &Pixel::ToRgb() {
    scheme_ = ColourParameters::ColourSchemes::RGB;
    return *this;
}

void Pixel::Copy(const Pixel &other) {
    scheme_ = other.scheme_;
    SetRed(other.GetRed());
    SetGreen(other.GetGreen());
    SetBlue(other.GetBlue());
}

bool Pixel::operator==(const Pixel &other) const {
    return abs(red_ - other.red_) < ColourParameters::EPS && abs(green_ - other.green_) < ColourParameters::EPS &&
           abs(blue_ - other.blue_) < ColourParameters::EPS;  // obnoxious doubles
}

bool Pixel::operator!=(const Pixel &other) const {
    return !(*this == other);
}

Pixel Pixel::operator-(const Pixel &other) const {
    Pixel temp;
    if (scheme_ == ColourParameters::ColourSchemes::Greyscale &&
        other.scheme_ == ColourParameters::ColourSchemes::Greyscale) {
        temp.scheme_ = ColourParameters::ColourSchemes::Greyscale;
    } else {
        temp.scheme_ = ColourParameters::ColourSchemes::RGB;
    }
    temp.SetRed(GetRed() - other.GetRed());
    temp.SetGreen(GetGreen() - other.GetGreen());
    temp.SetBlue(GetBlue() - other.GetBlue());
    return temp;
}

Pixel &Pixel::operator-=(const Pixel &other) {
    if (other.scheme_ == ColourParameters::ColourSchemes::RGB &&
        scheme_ == ColourParameters::ColourSchemes::Greyscale) {
        scheme_ = ColourParameters::ColourSchemes::RGB;
    }
    SetRed(GetRed() - other.GetRed());
    SetGreen(GetGreen() - other.GetGreen());
    SetBlue(GetBlue() - other.GetBlue());
    return *this;
}

Pixel Pixel::operator*(const Pixel &l) const {
    return Pixel(red_ * l.red_, green_ * l.green_, blue_ * l.blue_, scheme_);
}

Pixel Pixel::operator+(PixelParameters::Scalar l) const {
    return Pixel(red_ + l, green_ + l, blue_ + l, scheme_);
}

Pixel &Pixel::operator+=(PixelParameters::Scalar l) {
    SetRed(GetRed() + l);
    SetGreen(GetGreen() + l);
    SetBlue(GetBlue() + l);
    return *this;
}

Pixel::Pixel(ColourParameters::ColourType red, ColourParameters::ColourType green, ColourParameters::ColourType blue,
             ColourParameters::ColourSchemes scheme) {
    if (red == green == blue) {
        scheme_ = scheme;
    } else {
        scheme_ = ColourParameters::ColourSchemes::RGB;
    }
    SetRed(red);
    SetGreen(green);
    SetBlue(blue);
}

ColourParameters::ColourType Pixel::GetRed() const {
    return red_;
}

ColourParameters::ColourType Pixel::GetGreen() const {
    return green_;
}

ColourParameters::ColourType Pixel::GetBlue() const {
    return blue_;
}

Pixel &Pixel::ToNegative() {
    SetRed(1 - GetRed());
    SetGreen(1 - GetGreen());
    SetBlue(1 - GetBlue());
    return *this;
}

void Pixel::SetRed(ColourParameters::ColourType new_colour) {
    red_ = new_colour;
}

void Pixel::SetGreen(ColourParameters::ColourType new_colour) {
    green_ = new_colour;
}

void Pixel::SetBlue(ColourParameters::ColourType new_colour) {
    blue_ = new_colour;
}

Pixel::Pixel(const Pixel &other) : scheme_(other.scheme_), red_(other.red_), green_(other.green_), blue_(other.blue_) {
}

Pixel Pixel::operator-(PixelParameters::Scalar l) const {
    return Pixel(GetRed() - l, GetGreen() - l, GetBlue() - l);
}
Pixel &Pixel::operator-=(PixelParameters::Scalar l) {
    SetRed(GetRed() - l);
    SetGreen(GetGreen() - l);
    SetBlue(GetBlue() - l);
    return *this;
}
Pixel Pixel::operator/(PixelParameters::Scalar l) const {
    return Pixel(GetRed() / l, GetGreen() / l, GetBlue() / l);
}

ColourParameters::ColourType Pixel::GetValue() const {
    return (GetRed() + GetGreen() + GetBlue()) / 3;
}

Pixel Pixel::operator*=(const Pixel &l) {
    SetRed(GetRed() * l.GetRed());
    SetGreen(GetGreen() * l.GetGreen());
    SetBlue(GetBlue() * l.GetBlue());
    return *this;
}

Pixel Pixel::operator/(const Pixel &l) const {
    return Pixel(red_ / l.GetRed(), green_ / l.GetGreen(), blue_ / l.GetBlue());
}

Pixel &Pixel::operator/=(PixelParameters::Scalar l) {
    SetRed(GetRed() / l);
    SetGreen(GetGreen() / l);
    SetBlue(GetBlue() / l);
    return *this;
}

Pixel &Pixel::operator/=(const Pixel &l) {
    SetRed(GetRed() / l.GetRed());
    SetGreen(GetGreen() / l.GetGreen());
    SetBlue(GetBlue() / l.GetBlue());
    return *this;
}
