#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include "device.hpp"
#include <memory>
#include <vector>

namespace vklabs
{
    class Context
    {
    public:
        virtual std::shared_ptr<Device> CreateDevice(std::size_t index,
            std::vector<char const*> const& required_extensions) = 0;
        virtual ~Context() = default;

    };

}

#endif // CONTEXT_HPP_
