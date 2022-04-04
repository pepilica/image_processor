#include <cassert>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>

#include "../src/matrix.h"
#include "../src/pixel.h"
#include "../src/command_line_parser.h"
#include "../src/filter_pipeline_maker.h"
#include "../src/application.h"
#include "../src/image_manipulators.h"
#include "../src/poly.h"

using namespace std::literals;

using TestFunction = void(*)();

void TestWrapper(TestFunction func, std::string info) {
    try {
        std::cout << info << ":\t";
        std::cout.flush();
        func();
        std::cout << "[OK]" << std::endl;
        std::cout.flush();
    } catch (const std::exception& e) {
        std::cerr << "[FAIL]: " << e.what() << std::endl;
    }
}

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

void CommandLineParserTest() {
    CommandLineParser parser1;
    std::vector<std::string> params({"", "input.bmp", "output.bmp", "-filter1", "param", "param2", "-filter2"});
    char* argv1[3] = {nullptr, &*params[1].begin(), &*params[2].begin()};
    assert(parser1.Parse(3, argv1));
    assert(parser1.GetInput() == params[1]);
    assert(parser1.GetOutput() == params[2]);
    assert(!parser1.IsUsedForHelp());

    CommandLineParser parser2;
    char* argv2[1] = {nullptr};
    assert(!parser2.Parse(1, argv2));
    assert(parser2.GetInput().empty());
    assert(parser2.GetOutput().empty());
    assert(parser2.IsUsedForHelp());
    assert(parser2.GetDesiredFunction() == "-h");

    CommandLineParser parser3;
    char* argv3[2] = {nullptr, &*params[3].begin()};
    assert(!parser3.Parse(2, argv3));
    assert(parser3.GetInput().empty());
    assert(parser3.GetOutput().empty());
    assert(parser3.IsUsedForHelp());
    assert(parser3.GetDesiredFunction() == params[3]);

    CommandLineParser parser4;
    char* argv4[2] = {nullptr, &*params[3].begin()};
    assert(!parser4.Parse(2, argv3));
    assert(parser4.GetInput().empty());
    assert(parser4.GetOutput().empty());
    assert(parser4.IsUsedForHelp());
    assert(parser4.GetDesiredFunction() == params[3]);

    CommandLineParser parser5;
    char* argv5[7];
    std::transform(params.begin(), params.end(), std::begin(argv5), [](std::string& a){ return &*a.begin(); });
    assert(parser5.Parse(7, argv5));
    assert(parser5.GetInput() == params[1]);
    assert(parser5.GetOutput() == params[2]);
    assert(!parser5.IsUsedForHelp());
    assert(parser5.GetDesiredFunction().empty());
    assert(parser5.GetDescriptions().size() == 2);
}


void FilterPipelineMakerTest() {
    FilterPipelineMaker fpm;
    fpm.AddFilterCreator("Blur", &FilterMakers::MakeFastGaussianBlurFilter);  // & необязательно
    fpm.AddFilterCreator("Crop", &FilterMakers::MakeCropFilter);
    //    assert(fpm);
}

void ManipulatorTest() {
    FilterPipelineMaker fpm;
    fpm.AddFilterCreator("-sharp", &FilterMakers::MakeSharpeningFilter);

    FilterDescriptor fd;
    fd.SetFilterName("-sharp");
    fd.SetParams({});
    Manipulator* filter = fpm.MakeFilter(fd);
    assert(filter);
    Matrix<Pixel> image({
        {Pixel(1), Pixel(1), Pixel(1)},
        {Pixel(1), Pixel(1), Pixel(1)}
    });
    Matrix<Pixel> result = image;
    filter->Apply(image);
    assert(image == result);
    delete filter;
}


void BitmapTest() {

    Bitmap bitmap;

    std::string str1 = "../examples/output.bmp";
    assert(bitmap.load(str1.c_str()));

    std::string str2 = "../examples/notyan.jpg";
    assert(!bitmap.load(str2.c_str()));

    std::string str3 = "../examples/this_file_does_not_exist.bmp";
    assert(!bitmap.load(str3.c_str()));

    std::string str4 = "../examples/example.bmp";
    std::string output_str4 = "test_output.bmp";
    bitmap.load(str1.c_str());
    bitmap.save(output_str4.c_str());
    Bitmap bitmap_copy;
    bitmap_copy.load(output_str4.c_str());
    assert(bitmap.GetDIBHeader() == bitmap_copy.GetDIBHeader());
    assert(bitmap.GetDIBHeader() == bitmap_copy.GetDIBHeader());
    Matrix<Pixel> matrix1 = *bitmap.GetData();
    Matrix<Pixel> matrix2 = *bitmap_copy.GetData();
    assert(bitmap.GetDIBHeader() == bitmap_copy.GetDIBHeader());
    assert(bitmap.GetDIBHeader() == bitmap_copy.GetDIBHeader());
}

void PolyTest() {

    const Poly<int> poly0;                    // y = 0
    Poly<int> poly1({1, 0, 3});               // y = 1 + 0x + 3x^2
    const Poly<int> poly2({{0, 1}, {2, 3}});  // y = 1 + 3x^2

    assert(0 == poly0(1));
    assert(301 == poly1(10));
    assert(3458764513820540929 == poly1( (pow(2, 30)) ));

    assert(poly0 != poly1);
    assert(poly1 == poly2);

    assert(poly0 == poly1 - poly2);
    assert(Poly<int>({{0, 2}, {2, 6}}) == poly1 + poly2);

    poly1 += poly2;
    assert(poly1 == Poly<int>({2, 0, 6}));
    poly1 -= poly2;
    assert(poly1 == poly2);

    assert(Poly<int>({{0, 1}, {2, 6}, {4, 9}}) == poly1 * poly2);

    assert(Poly<int>({{0, -1}, {2, -3}}) == -poly2);

    std::stringstream string_stream;
    std::ostream& stream = string_stream;
    stream << poly1 << ", " << poly0;
    assert("y = 3x^2 + 1, y = 0" == string_stream.str());

    Poly<int> poly3 = poly0;
    assert(poly0 == poly3);

    poly3 = poly1;
    assert(poly1 == poly3);
}

void LagrangePolyTest() {
    PixelLagrangePolynomial<double> lagrange({0, 0.5, 1}, {0, 0.25, 1});
    assert(Poly<double>({std::pair<double, double>(2, 1)}) == lagrange);
}

void LeakCheckTest() {

}


void Testing() {
    std::cout << "Initializing tests..." << std::endl;
    std::cout << "---------------------" << std::endl << std::endl;

    TestWrapper(PixelTest, "Pixel logic test");
    TestWrapper(MatrixTest, "Matrix logic + convolution test");
    TestWrapper(CommandLineParserTest, "CommandLineParser test");
    TestWrapper(FilterPipelineMakerTest, "FilterPipelineMaker test");
    TestWrapper(ManipulatorTest, "Manipulator test");
    TestWrapper(BitmapTest, "Bitmap test");
    TestWrapper(PolyTest, "Polynomial test");
    TestWrapper(LagrangePolyTest, "Lagrange polynomial test");

    std::cout << std::endl << "---------------------" << std::endl;
    std::cout << "Tests passed successfully!" << std::endl;
}

int main() {
    Testing();
    return 0;
}