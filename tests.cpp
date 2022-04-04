#include <cassert>

#include "src/matrix.h"
#include "src/pixel.h"

void PixelTest() {
    Pixel pixel1;
    Pixel pixel2(1, 1, 1);
    pixel2 *= 0.5;
    assert(("Multiplication with scalar test", pixel1 == Pixel(0, 0, 0).ToGreyscale()));

    Pixel pixel3 = Pixel(0, 0.5, 1);
    pixel3.ToGreyscale();
    assert(("ToGreyscale() test", pixel3 == Pixel(0.5)));
}

void MatrixTest() {
    using ElementType = int64_t;

    Matrix<int64_t> matrix1;
    Matrix<int64_t> matrix2(std::vector<std::vector<int64_t>>({{1, 2, 3}, {4, 5, 6}}));
    matrix1 = matrix2;
    assert(("Matrix assignment test", matrix1 == matrix2));

    Matrix<ElementType> matrix3(std::vector<std::vector<int64_t>>({{1, 2, 3}, {4, 5, 6}}));
    assert(("Scalar multiplication test", matrix2 * matrix3 == 91));
    Matrix<ElementType> matrix4(std::vector<std::vector<int64_t>>({{1}, {4}}));
    bool is_wrong = false;
    try {
        ElementType result = matrix3 * matrix4;
    } catch (const MatrixParameters::SizesNotSuitableException& e) {
        is_wrong = true;
    }
    assert(("Bad check whether matrices have different sizes", is_wrong));

    Matrix<int> target(std::vector<std::vector<int>>({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}));
    Matrix<int> kernel(std::vector<std::vector<int>>({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}));
    assert(("Bad integer convolution", target.Convolution(kernel) == Matrix(std::vector<std::vector<int>>({{135, 168, 195},
                                                                                                           {252, 285, 312},
                                                                                                           {315, 348, 375}}))));

    Matrix<Pixel> target_colour(std::vector<std::vector<Pixel>>({{Pixel(0.01), Pixel(0.02), Pixel(0.03)},
                                                                 {Pixel(0.04), Pixel(0.05), Pixel(0.06)},
                                                                 {Pixel(0.07), Pixel(0.08), Pixel(0.09)}}));
    Matrix<Pixel> kernel_colour(std::vector<std::vector<Pixel>>({{Pixel(0.1), Pixel(0.2), Pixel(0.3)},
                                                                 {Pixel(0.4), Pixel(0.5), Pixel(0.6)},
                                                                 {Pixel(0.7), Pixel(0.8), Pixel(0.9)}}));

    assert(("Bad pixel convolution", target_colour.Convolution(kernel_colour) == Matrix(std::vector<std::vector<Pixel>>(
            {{Pixel(0.135), Pixel(0.168), Pixel(0.195)},
             {Pixel(0.252), Pixel(0.285), Pixel(0.312)},
             {Pixel(0.315), Pixel(0.348), Pixel(0.375)}}))));
}

void Testing() {
    std::cout << "Initializing tests..." << std::endl;

    PixelTest();
    std::cout << "Colour logic test successful" << std::endl;

    MatrixTest();
    std::cout << "Matrix logic + convolution test successful" << std::endl;

}
