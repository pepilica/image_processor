#ifndef IMAGE_PROCESSOR_PIXEL_H
#define IMAGE_PROCESSOR_PIXEL_H

#include <algorithm>
#include <string>

namespace ColourParameters {
    using ColourType = double;
    enum class ColourSchemes {
        Greyscale,
        RGB,
        Void
    };
    const double EPS = std::numeric_limits<double>::epsilon();
}

namespace PixelParameters {
    using Scalar = double;
}

class Pixel {
public:

    Pixel();
    explicit Pixel(ColourParameters::ColourType value);
    Pixel(ColourParameters::ColourType red,
          ColourParameters::ColourType green,
          ColourParameters::ColourType blue,
          ColourParameters::ColourSchemes scheme = ColourParameters::ColourSchemes::Greyscale);
    Pixel(const Pixel &other);

    std::string info() const;
    Pixel InGrayScalePixel() const;
    Pixel InRgbPixel() const;
    ColourParameters::ColourSchemes GetScheme() const;
    Pixel& ToGreyscale();
    Pixel& ToRgb();
    Pixel& ToNegative();

    Pixel &operator=(const Pixel &other);

    Pixel operator+(const Pixel &other) const;
    Pixel& operator+=(const Pixel &other);
    Pixel operator+(PixelParameters::Scalar l) const;
    Pixel& operator+=(PixelParameters::Scalar l);

    Pixel operator-(const Pixel &other) const;
    Pixel operator-(PixelParameters::Scalar l) const;
    Pixel& operator-=(const Pixel &other);
    Pixel& operator-=(PixelParameters::Scalar l);

    Pixel operator*(PixelParameters::Scalar l) const;
    Pixel operator*(const Pixel& l) const;
    Pixel& operator*=(PixelParameters::Scalar l);
    Pixel operator*=(const Pixel& l);

    Pixel operator/(PixelParameters::Scalar l) const;
    Pixel operator/(const Pixel& l) const;
    Pixel& operator/=(PixelParameters::Scalar l);
    Pixel& operator/=(const Pixel& l);

    bool operator==(const Pixel& other) const;
    bool operator!=(const Pixel& other) const;

    ColourParameters::ColourType GetValue() const;

    ColourParameters::ColourType GetRed() const;
    ColourParameters::ColourType GetGreen() const;
    ColourParameters::ColourType GetBlue() const;

    void SetRed(ColourParameters::ColourType new_colour);
    void SetGreen(ColourParameters::ColourType new_colour);
    void SetBlue(ColourParameters::ColourType new_colour);

    ~Pixel() = default;

protected:
    ColourParameters::ColourSchemes scheme_;
    ColourParameters::ColourType red_;
    ColourParameters::ColourType green_;
    ColourParameters::ColourType blue_;
    void Copy(const Pixel& other);
};


#endif //IMAGE_PROCESSOR_PIXEL_H
