#ifndef RENDER_HPP_
#define RENDER_HPP_

#include <memory>

class VulkanAPI;
class VulkanDevice;

class Render
{
public:
    Render(std::shared_ptr<VulkanDevice> device);
    void RenderFrame();

private:
    std::shared_ptr<VulkanAPI> videoapi_;
    std::shared_ptr<VulkanDevice> device_;

};

#endif // RENDER_HPP_