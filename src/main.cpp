#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

int main()
{
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "VulkanLabs";
    app_info.applicationVersion = 1;
    app_info.apiVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    VkInstance instance = nullptr;
    if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
    {
        std::cerr << "Failed to create instance!" << std::endl;
        return -1;
    }

    std::uint32_t physical_device_count = 0;
    if (vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr) != VK_SUCCESS)
    {
        std::cerr << "Failed to enumerate physical devices!" << std::endl;
        return -1;
    }

    std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
    vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());

    std::cout << "Physical device count: " << physical_device_count << std::endl;
    for (auto const& device : physical_devices)
    {
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(device, &device_properties);
#define PRINT_PROPERTY(x) \
        std::cout << (#x) << ": " << (x) << std::endl

        PRINT_PROPERTY(device_properties.apiVersion);
        PRINT_PROPERTY(device_properties.deviceID);
        PRINT_PROPERTY(device_properties.deviceName);
        PRINT_PROPERTY(device_properties.deviceType);
        PRINT_PROPERTY(device_properties.driverVersion);
        PRINT_PROPERTY(device_properties.vendorID);

    }

    if (!glfwInit())
        exit(EXIT_FAILURE);


    GLFWwindow* window = glfwCreateWindow(1280, 720, "VulkanLabs", nullptr, nullptr);
    while (!glfwWindowShouldClose(window))
    {

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
