#include "application.h"

namespace FilterMakers {

    Manipulator* MakeFastGaussianBlurFilter(const FilterDescriptor& fd) {
        if (fd.GetFilterName() != "-blur") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeBlurFilter");
        }
        if (fd.GetParams().size() > 1) {
            throw std::invalid_argument("invalid arguments number passed to MakeBlurFilter");
        }
        char *dummy;
        double sigma = 1.0;
        if (fd.GetParams().size() == 1) {
            sigma = std::strtod(fd.GetParams()[0].begin(), &dummy);
        }
        return new FastGaussianBlurFilter(sigma);
    }

    Manipulator* MakeCropFilter(const FilterDescriptor& fd) {
        if (fd.GetFilterName() != "-crop") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeCropFilter");
        }
        if (fd.GetParams().size() != 2) {
            throw std::invalid_argument("invalid arguments number passed to MakeCropFilter");
        }
        char* dummy;
        size_t width = std::strtol(fd.GetParams()[0].begin(), &dummy, 10);
        size_t height = std::strtol(fd.GetParams()[1].begin(), &dummy, 10);
        return new CropFilter(width, height);
    }

    Manipulator* MakeSharpeningFilter(const FilterDescriptor& fd) {
        if (fd.GetFilterName() != "-sharp") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeSharpeningFilter");
        }
        return new SharpeningFilter();
    }

    Manipulator* MakeEdgeDetectionFilter(const FilterDescriptor& fd) {
        if (fd.GetFilterName() != "-edge") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeEdgeDetectionFilter");
        }
        if (fd.GetParams().size() > 1) {
            throw std::invalid_argument("invalid arguments number passed to MakeEdgeDetectionFilter");
        }
        double threshold = 0;
        if (fd.GetParams().empty()) {
            threshold = 0.33;
        } else {
            char* dummy;
            threshold = std::strtod(fd.GetParams()[0].begin(), &dummy);
        }
        return new EdgeDetectionFilter(threshold);
    }

    Manipulator* MakeNegativeFilter(const FilterDescriptor &fd) {
        if (fd.GetFilterName() != "-neg") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeNegativeFilter");
        }
        if (!fd.GetParams().empty()) {
            throw std::invalid_argument("invalid arguments number passed to MakeNegativeFilter");
        }
        return new NegativeFilter();
    }

    Manipulator *MakeToGreyscaleBasicFilter(const FilterDescriptor &fd) {
        if (fd.GetFilterName() != "-gsbasic") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeToGreyscaleBasicFilter");
        }
        if (fd.GetParams().empty()) {
            throw std::invalid_argument("invalid arguments number passed to MakeToGreyscaleBasicFilter");
        }
        return new ToGreyscaleBasicFilter();
    }

    Manipulator *MakeToGreyscaleFilter(const FilterDescriptor &fd) {
        if (fd.GetFilterName() != "-gs") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeToGreyscaleFilter");
        }
        if (!fd.GetParams().empty()) {
            throw std::invalid_argument("invalid arguments number passed to MakeToGreyscaleFilter");
        }
        return new ToGreyscaleFilter();
    }

    Manipulator* MakeCurvesFilter(const FilterDescriptor &fd) {
        if (fd.GetFilterName() != "-curves") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeCurvesFilter");
        }
        if ((fd.GetParams().size() % 2) != 0) {
            throw std::invalid_argument("invalid arguments number passed to MakeCurvesFilter");
        }
        size_t size = fd.GetParams().size();
        const std::vector<std::string_view>& params = fd.GetParams();
        std::vector<PixelParameters::Scalar> xs(0);
        std::vector<PixelParameters::Scalar> ys(0);
        std::map<PixelParameters::Scalar, PixelParameters::Scalar> values;
        for (size_t i = 0; i < size / 2; ++i) {
            char* dummy;
            double x_i = std::strtod(params[2 * i].begin(), &dummy);
            double y_i = std::strtod(params[2 * i + 1].begin(), &dummy);
            auto ptr_i = values.find(x_i);
            if (ptr_i != values.end()) {
                throw std::invalid_argument("Identical x-coordinates found at MakeCurvesFilter");
            }
            values.insert({x_i, y_i});
        }
        auto ptr_0 = values.find(0);
        if (ptr_0 == values.end()) {
            values.insert({0, 0});
        }

        auto ptr_1 = values.find(1);
        if (ptr_1 == values.end()) {
            values.insert({1, 1});
        }

        for (auto& [x_i, y_i] : values) {
            xs.push_back(x_i);
            ys.push_back(y_i);
        }
        return new CurvesFilter(xs, ys);
    }

}

void Application::Configure() {
    GetFilterPipelineMaker().AddFilterCreator("-blur", FilterMakers::MakeFastGaussianBlurFilter);
    GetFilterPipelineMaker().AddFilterCreator("-crop", FilterMakers::MakeCropFilter);
    GetFilterPipelineMaker().AddFilterCreator("-sharp", FilterMakers::MakeSharpeningFilter);
    GetFilterPipelineMaker().AddFilterCreator("-edge", FilterMakers::MakeEdgeDetectionFilter);
    GetFilterPipelineMaker().AddFilterCreator("-neg", FilterMakers::MakeNegativeFilter);
    GetFilterPipelineMaker().AddFilterCreator("-gsbasic", FilterMakers::MakeToGreyscaleBasicFilter);
    GetFilterPipelineMaker().AddFilterCreator("-gs", FilterMakers::MakeToGreyscaleFilter);
    GetFilterPipelineMaker().AddFilterCreator("-curves", FilterMakers::MakeCurvesFilter);
    helpers_.insert({"-blur", FastGaussianBlurFilter::GetHelp});
    helpers_.insert({"-crop", CropFilter::GetHelp});
    helpers_.insert({"-sharp", SharpeningFilter::GetHelp});
    helpers_.insert({"-edge", EdgeDetectionFilter::GetHelp});
    helpers_.insert({"-neg", NegativeFilter::GetHelp});
    helpers_.insert({"-gsbasic", ToGreyscaleBasicFilter::GetHelp});
    helpers_.insert({"-gs", ToGreyscaleFilter::GetHelp});
    helpers_.insert({"-curves", CurvesFilter::GetHelp});
    helpers_.insert({"-h", GetHelp});
}

FilterPipelineMaker &Application::GetFilterPipelineMaker() {
    return filter_pipeline_maker_;
}

void Application::Run(int argc, char **argv) {
    try {
        CommandLineParser clm;
        // Testing(); // TODO: перенести тестирование в отдельную компоненту
        bool is_parsed = clm.Parse(argc, argv);
        std::cout << "Reading input..." << std::endl;
        if (!is_parsed) {
            if (clm.IsUsedForHelp()) {
                std::cout << GetHelper(clm.GetDesiredFunction())() << std::endl;
            } else {
                std::cout << WRONG_INPUT << std::endl;
            }
            return;
        }
        std::cout << "Parsed successfully" << std::endl;
        Bitmap input_bitmap;
        std::cout << "Loading file..." << std::endl;
        bool is_loaded = input_bitmap.load(clm.GetInput().begin());
        if (!is_loaded) {
            std::cout << "file could not be loaded or has wrong type" << std::endl;
            return;
        }
        std::cout << "Loaded successfully" << std::endl;
        FilterPipelineMaker& maker = GetFilterPipelineMaker();
        std::cout << "Creating pipeline..." << std::endl;
        FilterPipeline pipeline = maker.BuildPipeline(clm.GetDescriptions());
        std::cout << "Created successfully" << std::endl;
        std::cout << "Applying filters..." << std::endl;
        Bitmap output_bitmap = pipeline.Apply(input_bitmap);
        std::cout << "Applied successfully" << std::endl;
        std::cout << "Saving file..." << std::endl;
        bool is_saved = output_bitmap.save(clm.GetOutput().begin());
        if (!is_saved) {
            std::cout << "file could not be saved" << std::endl;
            return;
        }
        std::cout << "Result saved to " << clm.GetOutput() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Fatal failure! Caught exception: ";
        std::cerr << e.what();
    }
}

std::string Application::GetHelp() {
    return HELP;
}
Application::FilterHelpers Application::GetHelpers() const {
    return helpers_;
}

Application::FilterHelper Application::GetHelper(std::string_view query) const {
    auto helper_ptr = helpers_.find(query);
    if (helper_ptr != GetHelpers().end()) {
        return *helper_ptr->second;
    }
    return []() -> std::string { return WRONG_INPUT; };
}
