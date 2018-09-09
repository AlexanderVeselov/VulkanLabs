#ifndef VK_SHARED_OBJECT_HPP_
#define VK_SHARED_OBJECT_HPP_

#include <memory>

template <class T>
using VkSharedObject = std::shared_ptr<std::remove_pointer_t<T>>;

#endif // VK_SHARED_OBJECT_HPP_