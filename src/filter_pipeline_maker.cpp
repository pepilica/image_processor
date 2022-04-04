#include "filter_pipeline_maker.h"

Manipulator* FilterPipelineMaker::MakeFilter(const FilterDescriptor& fd) const {
    FilterMakerPtr maker = GetFilterMaker(fd.GetFilterName());
    if (!maker) {
        return nullptr;
    }

    return maker(fd);
}

FilterPipelineMaker::FilterMakerPtr FilterPipelineMaker::GetFilterMaker(std::string_view name) const {
    auto ix = filter_creators_.find(name);
    if (ix == filter_creators_.end()) {
        return nullptr;
    }
    return ix->second;
}
FilterPipeline FilterPipelineMaker::BuildPipeline(const std::vector<FilterDescriptor>& descriptions) {
    FilterPipeline pipeline;
    for (const FilterDescriptor& descriptor : descriptions) {
        pipeline.GetPipeline().push_back(MakeFilter(descriptor));
    }
    return pipeline;
}
