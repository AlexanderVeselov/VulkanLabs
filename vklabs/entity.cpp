#include "entity.hpp"

REGISTER_ENTITY_CLASS(Entity, some_entity);

#include <iostream>

Entity::Entity(EntityManager & entity_manager, EntityId entity_id)
    : entity_id_(entity_id)
    , entity_manager_(entity_manager)
{
}

void Entity::Ping()
{
    printf("Ping\n");
}
