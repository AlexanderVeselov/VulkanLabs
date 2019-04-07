#include "entity_manager.hpp"
#include "component_manager.hpp"
#include "entity.hpp"
#include <string>
#include <map>

namespace
{
    struct EntityFactoryMap
    {
    private:
        std::unordered_map<std::string,
            Entity* (*)(EntityManager &, ComponentManager &, EntityId)>
            entity_factory_map_;

    public:
        static decltype(entity_factory_map_) & GetMap()
        {
            static EntityFactoryMap map;
            return map.entity_factory_map_;
        }

    };

}

void RegisterEntityFactoryFunc(char const* entity_type, Entity* (*func)(EntityManager &, ComponentManager &, EntityId))
{
    EntityFactoryMap::GetMap().emplace(entity_type, func);
}

EntityManager::EntityManager(ComponentManager & component_manager)
    : component_manager_(component_manager)
{

}

std::shared_ptr<Entity> EntityManager::CreateEntity(char const* entity_type)
{
    auto it = EntityFactoryMap::GetMap().find(entity_type);

    if (it == EntityFactoryMap::GetMap().end())
    {
        throw std::runtime_error("Failed to create entity: Unregistered entity type!");
    }

    Entity* entity = it->second(*this, component_manager_, next_entity_id_++);
    return std::shared_ptr<Entity>(entity);
}
