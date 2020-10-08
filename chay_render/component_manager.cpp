#include "component_manager.hpp"

namespace
{
    class ComponentPoolFactoryMap
    {
    private:
        std::unordered_map<std::string, ComponentPool* (*)()> component_pool_factory_map_;

    public:
        static decltype(component_pool_factory_map_) & GetMap()
        {
            static ComponentPoolFactoryMap map;
            return map.component_pool_factory_map_;
        }

    };
}

void RegisterComponentPoolFactory(char const* type_name, ComponentPool* (*factory_fun)())
{
    ComponentPoolFactoryMap::GetMap().emplace(type_name, factory_fun);
}

ComponentManager::ComponentManager()
{
    CreateComponentPools();
}

void ComponentManager::CreateComponentPools()
{
    for (auto factory : ComponentPoolFactoryMap::GetMap())
    {
        ComponentPool* pool = factory.second();
        component_pools_.emplace(pool->GetComponentTypeId(), pool);
    }
}
