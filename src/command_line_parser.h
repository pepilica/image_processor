#ifndef PROJECT_COMMAND_LINE_PARSER_H
#define PROJECT_COMMAND_LINE_PARSER_H

#include <iostream>
#include <string>
#include <string_view>
#include <cstddef>
#include <vector>

class FilterDescriptor {
public:
    FilterDescriptor() : filter_name_(), params_(std::vector<std::string_view>(0)){};

    void SetFilterName(std::string_view new_name);
    void SetParams(const std::vector<std::string_view>& params);
    void AddParameter(std::string_view new_param);
    std::string_view GetFilterName() const;
    std::vector<std::string_view> GetParams() const;

protected:
    std::string_view filter_name_;
    std::vector<std::string_view> params_;
};

class CommandLineParser {
public:
    static const size_t MIN_ARG_NUM = 3;
    static const size_t INPUT_FILE_INDEX = 1;
    static const size_t OUTPUT_FILE_INDEX = 2;
    static const size_t HELP_INDEX = 1;

    using Descriptions = std::vector<FilterDescriptor>;

public:
    CommandLineParser();

    bool Parse(int argc, char* argv[]);
    std::vector<FilterDescriptor> GetDescriptions() const;
    std::string_view GetInput() const {
        return input_file_name_;
    }
    std::string_view GetOutput() const {
        return output_file_name_;
    }
    bool IsUsedForHelp() const;
    std::string GetDesiredFunction() const;

protected:
    std::vector<FilterDescriptor> descriptions_;
    std::string_view input_file_name_;
    std::string_view output_file_name_;
    bool is_used_for_help_;
    std::string desired_function_;
};

#endif  // PROJECT_COMMAND_LINE_PARSER_H
