#include "entity.hpp"
#include "entity_manager.hpp"
#include "component_manager.hpp"

REGISTER_ENTITY_CLASS(Entity, some_entity);

Entity::Entity(EntityManager & entity_manager, ComponentManager & component_manager, EntityId entity_id)
    : entity_id_(entity_id)
    , entity_manager_(entity_manager)
    , component_manager_(component_manager)
{
}

void Entity::Ping()
{
    printf("Ping\n");
}
