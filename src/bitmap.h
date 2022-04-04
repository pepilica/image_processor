#ifndef IMAGE_PROCESSOR_BITMAP_H
#define IMAGE_PROCESSOR_BITMAP_H

#include "matrix.h"
#include "pixel.h"

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>

class Bitmap {
public:
    Bitmap(const Bitmap& other);
    Bitmap(Bitmap&& other) = default;
    Bitmap() : data_(nullptr), bmp_header_(), dib_header_(){};

    struct BMPHeader {
        uint16_t signature;
        uint32_t bmp_size;
        uint16_t dummy_1;
        uint16_t dummy_2;
        uint32_t offset;

        bool operator==(const BMPHeader& other) const;
    } __attribute__((packed));

    struct DIBHeader {
        uint32_t header_size;
        int32_t width;
        int32_t height;
        uint16_t number_color_planes;
        uint16_t bits_per_pixel;
        uint32_t compression;
        uint32_t image_size;
        int32_t horizontal_resolution;
        int32_t vertical_resolution;
        uint32_t number_colors;
        uint32_t number_important_colors;

        bool operator==(const DIBHeader& other) const;
    } __attribute__((packed));

    struct PrimitivePixel {
        uint8_t blue;
        uint8_t green;
        uint8_t red;

        PrimitivePixel(uint8_t r, uint8_t g, uint8_t b) {
            blue = b;
            green = g;
            red = r;
        };

        PrimitivePixel() {
            blue = 0;
            green = 0;
            red = 0;
        };

    } __attribute__((packed));

    bool load(std::istream& istr);
    bool load(const char* file_name);
    bool save(std::ofstream& istr);
    bool save(const char* file_name);
    Matrix<Pixel>* GetData();

    Bitmap& operator=(const Bitmap& other) = delete;
    Bitmap& operator=(Bitmap&& other) = default;
    ~Bitmap() = default;

    static bool CheckBMPHeader(const BMPHeader& header);  // true, если header - хороший, false иначе
    static bool CheckDIBHeader(const DIBHeader& header);

    DIBHeader GetDIBHeader() const;  // true, если header - хороший, false иначе
    BMPHeader GetBMPHeader() const;

protected:
    std::unique_ptr<Matrix<Pixel>> data_;
    BMPHeader bmp_header_;
    DIBHeader dib_header_;
};

#endif
