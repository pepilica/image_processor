#include "filter_pipeline.h"

Bitmap FilterPipeline::Apply(const Bitmap& PicStream) {
    Bitmap temp = PicStream;
    for (const Manipulator* manipulator : pipeline_) {
        if (manipulator != nullptr) {
            manipulator->Apply(*temp.GetData());
        }
    }
    return temp;
}

FilterPipeline::~FilterPipeline() {
    for (Manipulator* i : pipeline_) {
        delete i;
    }
}

FilterPipeline::Pipeline& FilterPipeline::GetPipeline() {
    return pipeline_;
}
FilterPipeline::FilterPipeline(FilterPipeline&& other) {
    std::swap(pipeline_, other.pipeline_);
}
