#ifndef SWAPCHAIN_HPP_
#define SWAPCHAIN_HPP_

#include "image_format.hpp"
#include <cstdint>

namespace vklabs
{
    class Swapchain
    {
    public:
        virtual ~Swapchain() = default;

        virtual void Present() const = 0;
        //virtual void ResizeBuffers(std::uint32_t width, std::uint32_t height, ImageFormat format) = 0;
    };
}

#endif // SWAPCHAIN_HPP_
