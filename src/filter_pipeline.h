#ifndef IMAGE_PROCESSOR_FILTER_PIPELINE_H
#define IMAGE_PROCESSOR_FILTER_PIPELINE_H

#include "bitmap.h"
#include "command_line_parser.h"
#include "image_manipulators.h"

#include <vector>

class FilterPipeline {
public:
    using Pipeline = std::vector<Manipulator*>;

    FilterPipeline() : pipeline_() {};
    Bitmap Apply(const Bitmap& PicStream);
    Pipeline& GetPipeline();
    FilterPipeline(const FilterPipeline& other) = delete;
    FilterPipeline& operator=(const FilterPipeline& other) = delete;
    FilterPipeline& operator=(FilterPipeline&& other) = default;
    FilterPipeline(FilterPipeline&& other);

    ~FilterPipeline();

protected:
    Pipeline pipeline_;
};

#endif
