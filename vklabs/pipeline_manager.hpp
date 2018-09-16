#ifndef PIPELINE_MANAGER_HPP_
#define PIPELINE_MANAGER_HPP_

#include "pipeline.hpp"
#include <memory>

class PipelineManager
{
public:
    virtual std::shared_ptr<Pipeline> CreatePipeline() const = 0;
};

#endif // PIPELINE_MANAGER_HPP_