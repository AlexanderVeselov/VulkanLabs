#include "component.hpp"
#include <typeinfo>

Component::Component(EntityId entity_id)
    : entity_id_(entity_id)
{
}

ComponentTypeId Component::GetTypeId()
{
    return reinterpret_cast<ComponentTypeId>(typeid(*this).name());
}
