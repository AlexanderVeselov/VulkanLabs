#include "application.hpp"
#include "vk_context.hpp"
#include <stdexcept>
#include <iostream>

Application::Application(AppSettings const& settings)
    : settings_(settings)
    , window_(nullptr, glfwDestroyWindow)
    , context_(nullptr)
{
    if (!glfwInit())
    {
        throw std::runtime_error("glfwInit() failed");
    }

    // Create main window
    window_.reset(glfwCreateWindow(settings.width, settings.height,
        settings_.app_name.c_str(), nullptr, nullptr));

    // Get GLFW requested extensions
    std::uint32_t glfw_extension_count;
    char const** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    context_.reset(new VkContext(settings, glfw_extensions, glfw_extension_count));

    if (!window_.get())
    {
        throw std::runtime_error("Failed to create GLFW window!");
    }

    glfwMakeContextCurrent(window_.get());

}

void Application::Run()
{
    while (!glfwWindowShouldClose(window_.get()))
    {
        // Main loop
        glfwSwapBuffers(window_.get());
        glfwPollEvents();
    }
}

Application::~Application()
{
    glfwTerminate();
}
