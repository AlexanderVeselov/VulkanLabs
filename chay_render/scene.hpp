#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "shaders/common.h"
#include <vector>

class Scene
{
public:

private:
    std::vector<std::uint32_t> index_buffer_;
    std::vector<Vertex> vertex_buffer_;
};

#endif // SCENE_HPP_
