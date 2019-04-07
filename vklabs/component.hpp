#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

#include "entity.hpp"
#include <string>

typedef std::size_t ComponentTypeId;

class Component
{
public:
    Component(EntityId entity_id);
    ComponentTypeId GetTypeId();
    EntityId GetEntityId() const { return entity_id_; }

protected:
    friend class ComponentManager;
    EntityId entity_id_;

};

#endif // COMPONENT_HPP_
