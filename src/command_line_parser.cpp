#include "command_line_parser.h"

bool CommandLineParser::Parse(int argc, char* argv[]) {
    if (argc < MIN_ARG_NUM) {
        if (argc == 2) {
            if (argv[HELP_INDEX][0] == '-') {
                is_used_for_help_ = true;
                desired_function_ = argv[HELP_INDEX];
            }
            return false;
        }
        is_used_for_help_ = true;
        desired_function_ = "-h";
        return false;
    }
    input_file_name_ = argv[INPUT_FILE_INDEX];
    output_file_name_ = argv[OUTPUT_FILE_INDEX];
    if (argc > MIN_ARG_NUM) {
        FilterDescriptor current_descriptor;
        bool is_void = true;
        for (size_t i = 3; i < argc; ++i) {
            if (argv[i][0] == '-') {
                if (is_void) {
                    is_void = false;
                } else {
                    descriptions_.push_back(current_descriptor);
                    current_descriptor = FilterDescriptor();
                }
                current_descriptor.SetFilterName({argv[i], strlen(argv[i])});
            } else {
                if (is_void) {
                    return false;
                }
                current_descriptor.AddParameter({argv[i], strlen(argv[i])});
            }
        }
        descriptions_.push_back(current_descriptor);
    }
    return true;
}

CommandLineParser::CommandLineParser()
    : descriptions_(std::vector<FilterDescriptor>(0)), is_used_for_help_(false), desired_function_() {
}

std::vector<FilterDescriptor> CommandLineParser::GetDescriptions() const {
    return descriptions_;
}

bool CommandLineParser::IsUsedForHelp() const {
    return is_used_for_help_;
}
std::string CommandLineParser::GetDesiredFunction() const {
    return desired_function_;
}

void FilterDescriptor::SetFilterName(std::string_view new_name) {
    filter_name_ = new_name;
}

void FilterDescriptor::SetParams(const std::vector<std::string_view>& params) {
    params_ = params;
}

void FilterDescriptor::AddParameter(std::string_view new_param) {
    params_.push_back(new_param);
}

std::string_view FilterDescriptor::GetFilterName() const {
    return filter_name_;
}

std::vector<std::string_view> FilterDescriptor::GetParams() const {
    return params_;
}
