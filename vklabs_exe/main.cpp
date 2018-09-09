#include "application.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    AppSettings settings;
    settings.width = 1280;
    settings.height = 720;
    settings.app_name = "VulkanLabs";

    try
    {
        vklabs::Application application(settings);
        application.Run();

    }
    catch (std::exception& ex)
    {
        std::cerr << "Caught exception:\n" << ex.what() << std::endl;
        return -1;
    }

    return 0;
}
