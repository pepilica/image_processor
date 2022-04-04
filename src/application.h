#ifndef PROJECT_APPLICATION_H
#define PROJECT_APPLICATION_H

#include "command_line_parser.h"
#include "filter_pipeline.h"
#include "filter_pipeline_maker.h"
#include "image_manipulators.h"

#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

namespace FilterMakers {
Manipulator* MakeFastGaussianBlurFilter(const FilterDescriptor& fd);
Manipulator* MakeCropFilter(const FilterDescriptor& fd);
Manipulator* MakeSharpeningFilter(const FilterDescriptor& fd);
Manipulator* MakeEdgeDetectionFilter(const FilterDescriptor& fd);
Manipulator* MakeNegativeFilter(const FilterDescriptor& fd);
Manipulator* MakeToGreyscaleBasicFilter(const FilterDescriptor& fd);
Manipulator* MakeToGreyscaleFilter(const FilterDescriptor& fd);
Manipulator* MakeCurvesFilter(const FilterDescriptor& fd);
}

namespace {
static const std::string HELP =
    "Usage:\n"
    "[input_file_path] [output_file_path] [-filter] {parameters}\n\n"
    "input_file_path: Path to the file to be processed,\n"
    "output_file_path: Path to the processed result.\n"
    "To get the filters' options, type \"image_processor [-filter_name] \"";

static const std::string WRONG_INPUT = "wrong input type, enter \"filter_processor -h\" to get help";
}

class Application {
public:
    typedef std::string (*FilterHelper)();
    using FilterHelpers = std::unordered_map<std::string_view, FilterHelper>;
    Application() : filter_pipeline_maker_(){};
    void Configure();
    void Run(int argc, char* argv[]);
    static std::string GetHelp();
    FilterHelpers GetHelpers() const;
    FilterHelper GetHelper(std::string_view query) const;

protected:
    FilterPipelineMaker& GetFilterPipelineMaker();
    FilterPipelineMaker filter_pipeline_maker_;
    FilterHelpers helpers_;
};

#endif  // PROJECT_APPLICATION_H
