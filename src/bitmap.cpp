#include "bitmap.h"

bool Bitmap::load(const char* file_name) {
    std::string str(file_name);
    std::ifstream file;
    file.open(str, std::ios_base::in | std::ios_base::binary);
    if (!file.is_open()) {
        return false;
    }
    bool status = load(file);
    return status;
}

bool Bitmap::load(std::istream& istr) {
    BMPHeader bmp_header;
    istr.read(reinterpret_cast<char *>(&bmp_header), sizeof(bmp_header));
    if (!CheckBMPHeader(bmp_header)) {
        return false;
    }

    DIBHeader dib_header;
    istr.read(reinterpret_cast<char *>(&dib_header), sizeof(dib_header));
    if (!CheckDIBHeader(dib_header)) {
        return false;
    }

    data_ = std::make_unique<Matrix<Pixel>>(
        Matrix<Pixel>(static_cast<uint64_t>(dib_header.width), static_cast<uint64_t>(dib_header.height)));
    size_t padding = (4 - (dib_header.width * dib_header.bits_per_pixel / 8) % 4) % 4;
    for (size_t y = 0; y < dib_header.height; ++y) {
        for (size_t x = 0; x < dib_header.width; ++x) {
            PrimitivePixel temp{};
            istr.read(reinterpret_cast<char *>(&temp), sizeof(temp));
            data_->GetElement(x, dib_header.height - y - 1) =
                Pixel(temp.red / 256.0, temp.green / 256.0, temp.blue / 256.0);
        }
        istr.ignore(padding);
    }

    bmp_header_ = bmp_header;
    dib_header_ = dib_header;

    return true;
}

bool Bitmap::CheckBMPHeader(const Bitmap::BMPHeader& header) {
    return (header.signature == 0x4d42);
}

bool Bitmap::CheckDIBHeader(const Bitmap::DIBHeader& header) {
    return (header.bits_per_pixel == 24 && header.header_size == 40 && header.compression == 0 &&
            header.number_color_planes == 1);
}

bool Bitmap::save(const char* file_name) {
    std::string str(file_name);
    std::ofstream file;
    file.open(str, std::ios_base::out | std::ios_base::binary);
    if (!file.is_open()) {
        return false;
    }
    return save(file);
}

bool Bitmap::save(std::ofstream& istr) {
    dib_header_.width = data_->GetWidth();
    dib_header_.height = data_->GetHeight();
    size_t padding = (4 - (dib_header_.width * dib_header_.bits_per_pixel / 8) % 4) % 4;
    dib_header_.image_size = (dib_header_.width * dib_header_.bits_per_pixel / 8 + padding) * dib_header_.height;
    bmp_header_.bmp_size = dib_header_.image_size + bmp_header_.offset;

    unsigned char bmp_padding[3] = {0, 0, 0};

    istr.write(reinterpret_cast<char *>(&bmp_header_), sizeof(bmp_header_));
    istr.write(reinterpret_cast<char *>(&dib_header_), sizeof(dib_header_));
    for (size_t y = 0; y < dib_header_.height; ++y) {
        for (size_t x = 0; x < dib_header_.width; ++x) {
            const Pixel &temp = data_->GetElement(x, dib_header_.height - y - 1);
            ColourParameters::ColourType lo = 0.0;
            ColourParameters::ColourType hi = 1.0;
            PrimitivePixel temp1 = {static_cast<uint8_t>(std::round(std::clamp(temp.GetRed(), lo, hi) * 255)),
                                    static_cast<uint8_t>(std::round(std::clamp(temp.GetGreen(), lo, hi) * 255)),
                                    static_cast<uint8_t>(std::round(std::clamp(temp.GetBlue(), lo, hi) * 255))};
            istr.write(reinterpret_cast<char *>(&temp1), sizeof(temp1));
        }
        istr.write(reinterpret_cast<char *>(bmp_padding), padding);
    }
    return true;
}

Matrix<Pixel> *Bitmap::GetData() {
    return &(*data_);
}

Bitmap::Bitmap(const Bitmap& other) {
    data_ = std::make_unique<Matrix<Pixel>>(Matrix<Pixel>());
    *data_ = *(other.data_);
    dib_header_ = other.dib_header_;
    bmp_header_ = other.bmp_header_;
}
Bitmap::DIBHeader Bitmap::GetDIBHeader() const {
    return dib_header_;
}
Bitmap::BMPHeader Bitmap::GetBMPHeader() const {
    return bmp_header_;
}

bool Bitmap::BMPHeader::operator==(const Bitmap::BMPHeader& other) const {
    return (signature == other.signature && bmp_size == other.bmp_size && dummy_1 == other.dummy_1 &&
            dummy_2 == other.dummy_2 && offset == other.offset);
}
bool Bitmap::DIBHeader::operator==(const Bitmap::DIBHeader& other) const {
    return (header_size == other.header_size && width == other.width && height == other.height &&
            number_color_planes == other.number_color_planes && bits_per_pixel == other.bits_per_pixel &&
            compression == other.compression && image_size == other.image_size &&
            horizontal_resolution == other.horizontal_resolution && vertical_resolution == other.vertical_resolution &&
            number_colors == other.number_colors && number_important_colors == other.number_important_colors);
}
