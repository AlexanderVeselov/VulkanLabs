#include "entity_manager.hpp"
#include "entity.hpp"
#include <string>
#include <unordered_map>

namespace
{
    std::unordered_map<std::string, Entity* (*)(EntityManager &, EntityId)> g_entity_type_2_factory_func;
}

void RegisterEntityFactoryFunc(char const* entity_type, Entity* (*func)(EntityManager &, EntityId))
{
    g_entity_type_2_factory_func.emplace(entity_type, func);
}

std::shared_ptr<Entity> EntityManager::CreateEntity(char const* entity_type)
{
    auto it = g_entity_type_2_factory_func.find(entity_type);

    if (it == g_entity_type_2_factory_func.end())
    {
        throw std::runtime_error("Failed to create entity: Unregistered entity type!");
    }

    Entity* entity = it->second(*this, next_entity_id_++);
    std::shared_ptr<Entity> ptr;
    ptr.reset(entity);
    return ptr;
}
