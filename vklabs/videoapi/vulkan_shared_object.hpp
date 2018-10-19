#ifndef VULKAN_SHARED_OBJECT_HPP_
#define VULKAN_SHARED_OBJECT_HPP_

#include <memory>

template <class T>
using VulkanSharedObject = std::shared_ptr<std::remove_pointer_t<T>>;

#endif // VULKAN_SHARED_OBJECT_HPP_