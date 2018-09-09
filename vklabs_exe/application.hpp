#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include "context.hpp"
#include "app_settings.hpp"
#include <GLFW/glfw3.h>
#include <memory>

namespace vklabs
{
    class Application
    {
    public:
        Application(AppSettings const& settings);
        ~Application();
        void Run();

    private:
        AppSettings settings_;
        std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> window_;
        std::shared_ptr<Context> context_;

    };

}

#endif // APPLICATION_HPP_
