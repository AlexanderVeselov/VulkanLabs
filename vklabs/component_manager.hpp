#ifndef COMPONENT_MANAGER_HPP_
#define COMPONENT_MANAGER_HPP_

#include "component.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeinfo>

class ComponentPool
{
public:
    ComponentPool(ComponentTypeId component_type_id, std::size_t component_size)
        : component_type_id_(component_type_id)
        , component_size_(component_size)
    {}

    ComponentTypeId GetComponentTypeId() const { return component_type_id_; }
private:
    ComponentTypeId component_type_id_;
    std::size_t component_size_;
    std::vector<std::uint8_t> pool_;

};

class ComponentManager
{
public:
    ComponentManager();

private:
    void CreateComponentPools();
    std::unordered_map<ComponentTypeId, std::unique_ptr<ComponentPool>> component_pools_;

};

void RegisterComponentPoolFactory(char const* type_name, ComponentPool* (*factory_fun)());

#define REGISTER_COMPONENT_CLASS(CLASS, NAME) \
    class CLASS##_registerer \
    { \
    public: \
        CLASS##_registerer() \
        { \
            RegisterComponentPoolFactory(#NAME, []() \
            { \
                return new ComponentPool(reinterpret_cast<ComponentTypeId>(typeid(CLASS).name()), sizeof(CLASS)); \
            }); \
        } \
    }; \
    static CLASS##_registerer g_##CLASS##_registerer;

#endif // COMPONENT_MANAGER_HPP_
