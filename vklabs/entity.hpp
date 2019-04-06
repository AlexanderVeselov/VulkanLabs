#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "entity_manager.hpp"
#include <string>

class Entity
{
public:
    Entity(EntityManager & entity_manager, EntityId entity_id);
    void Ping();

protected:
    EntityId entity_id_;
    EntityManager & entity_manager_;
};

#endif // ENTITY_HPP_
