#ifndef PROJECT_FILTER_PIPELINE_MAKER_H
#define PROJECT_FILTER_PIPELINE_MAKER_H

#include <cstdlib>
#include <map>
#include <string_view>

#include "command_line_parser.h"
#include "filter_pipeline.h"
#include "image_manipulators.h"

class FilterPipelineMaker {
public:
    using FilterMakerPtr = Manipulator* (*)(const FilterDescriptor&);
    using FilterCreators = std::map<std::string_view, FilterMakerPtr>;

public:
    void AddFilterCreator(std::string_view filter_name, FilterMakerPtr filter_maker) {
        filter_creators_.insert({filter_name, filter_maker});
    }

    Manipulator* MakeFilter(const FilterDescriptor& fd) const;
    FilterMakerPtr GetFilterMaker(std::string_view name) const;
    FilterPipeline BuildPipeline(const std::vector<FilterDescriptor>& descriptions);

protected:
    FilterCreators filter_creators_;
};

#endif  // PROJECT_FILTER_PIPELINE_MAKER_H
