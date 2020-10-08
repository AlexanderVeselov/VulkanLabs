#ifndef COMPONENT_MANAGER_HPP_
#define COMPONENT_MANAGER_HPP_

#include "component.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <stdexcept>

constexpr std::size_t kComponentPoolGrowCount = 1024u;

class ComponentPool
{
public:
    ComponentPool(ComponentTypeId component_type_id, std::size_t component_size)
        : component_type_id_(component_type_id)
        , component_size_(component_size)
    {
        pool_.resize(component_size_ * kComponentPoolGrowCount);
    }

    ComponentTypeId GetComponentTypeId() const { return component_type_id_; }
    // Don't keep this pointer for a long time!
    // This operation can invalidate iterators on the next allocation
    void* AllocateComponent(EntityId id)
    {
        auto it = entity_id_2_pool_index_.find(id);
        if (it != entity_id_2_pool_index_.end())
        {
            throw std::runtime_error("Component is already allocated!");
        }

        // Grow pool if we don't have enough space
        if (component_size_ * component_count_ > pool_.size())
        {
            pool_.resize(pool_.size() + component_size_ * kComponentPoolGrowCount);
        }

        entity_id_2_pool_index_.emplace(id, component_count_);

        return pool_.data() + component_size_ * (component_count_++);
    }

    void* GetComponent(EntityId id)
    {
        auto it = entity_id_2_pool_index_.find(id);
        if (it == entity_id_2_pool_index_.end())
        {
            throw std::runtime_error("Failed to find component!");
        }

        return pool_.data() + component_size_ * it->second;
    }

private:
    ComponentTypeId component_type_id_;
    std::size_t component_size_;
    std::size_t component_count_ = 0;
    std::vector<std::uint8_t> pool_;
    std::unordered_map<EntityId, std::size_t> entity_id_2_pool_index_;

};

class ComponentManager
{
public:
    ComponentManager();
    template <class T>
    T* CreateComponent(EntityId entity_id);

    template <class T>
    T* GetComponent(EntityId entity_id);

private:
    void CreateComponentPools();
    std::unordered_map<ComponentTypeId, std::unique_ptr<ComponentPool>> component_pools_;

};

template <class T>
ComponentTypeId GetComponentTypeId()
{
    return reinterpret_cast<ComponentTypeId>(typeid(T).name());
}

template <class T>
T* ComponentManager::CreateComponent(EntityId entity_id)
{
    ComponentTypeId type_id = GetComponentTypeId<T>();
    auto it = component_pools_.find(type_id);
    if (it == component_pools_.end())
    {
        throw std::runtime_error("Failed to create component: component type is not registered");
    }

    auto & pool = it->second;
    T* component = static_cast<T*>(pool->AllocateComponent(entity_id));
    component->entity_id_ = entity_id;
    return component;
}

template <class T>
T* ComponentManager::GetComponent(EntityId entity_id)
{
    ComponentTypeId type_id = GetComponentTypeId<T>();
    auto it = component_pools_.find(type_id);
    if (it == component_pools_.end())
    {
        throw std::runtime_error("Failed to get component: component type is not registered");
    }

    auto & pool = it->second;
    return static_cast<T*>(pool->GetComponent(entity_id));
}

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
