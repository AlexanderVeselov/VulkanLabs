#ifndef VK_PIPELINE_MANAGER_HPP_
#define VK_PIPELINE_MANAGER_HPP_

#include "pipeline_manager.hpp"

class VkPipelineManager : PipelineManager
{
public:
    std::shared_ptr<Pipeline> CreatePipeline() const override;
};

#endif // VK_PIPELINE_MANAGER_HPP_