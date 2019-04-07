#ifndef ENTITY_MANAGER_HPP_
#define ENTITY_MANAGER_HPP_

#include "entity.hpp"

#include <vector>
#include <memory>

class ComponentManager;

class EntityManager
{
public:
    EntityManager(ComponentManager & component_manager);
    std::shared_ptr<Entity> CreateEntity(char const* entity_type);

private:
    ComponentManager & component_manager_;
    // TODO: very simple version
    std::vector<std::shared_ptr<Entity>> entity_pool_;
    EntityId next_entity_id_ = 0;

};

void RegisterEntityFactoryFunc(char const* entity_type, Entity* (*func)(EntityManager &, ComponentManager &, EntityId));

#define REGISTER_ENTITY_CLASS(CLASS, NAME) \
    class CLASS##_registerer \
    { \
    public: \
        CLASS##_registerer() \
        { \
            RegisterEntityFactoryFunc(#NAME, [](EntityManager & entity_manager, ComponentManager & component_manager, EntityId entity_id) \
            { \
                return new CLASS(entity_manager, component_manager, entity_id); \
            }); \
        } \
    }; \
    static CLASS##_registerer g_##CLASS##_registerer;

#endif // ENTITY_MANAGER_HPP_
