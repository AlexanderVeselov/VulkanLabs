#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <string>

class EntityManager;
class ComponentManager;

typedef std::uint64_t EntityId;

class Entity
{
public:
    Entity(EntityManager & entity_manager, ComponentManager & component_manager, EntityId entity_id);
    void Ping();

protected:
    EntityId entity_id_;
    EntityManager & entity_manager_;
    ComponentManager & component_manager_;
};

#endif // ENTITY_HPP_
