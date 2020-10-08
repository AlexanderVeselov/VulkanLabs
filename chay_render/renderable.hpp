#ifndef RENDERABLE_HPP_
#define RENDERABLE_HPP_

#include "entity.hpp"

class Renderable : public Entity
{
public:
    Renderable(EntityManager & entity_manager, ComponentManager & component_manager, EntityId entity_id)
        : Entity(entity_manager, component_manager, entity_id)
    {}

};

#endif // RENDERABLE_HPP_
