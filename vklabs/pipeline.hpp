#ifndef PIPELINE_HPP_
#define PIPELINE_HPP_

#include "shader.hpp"

struct PipelineDesc
{
    std::shared_ptr<Shader> vertex_shader;
    std::shared_ptr<Shader> pixel_shader;

};

class Pipeline
{
public:

};

#endif // PIPELINE_HPP_
