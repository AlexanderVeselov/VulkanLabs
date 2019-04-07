#include "component.hpp"

ComponentTypeId Component::GetTypeId()
{
    return reinterpret_cast<ComponentTypeId>(typeid(*this).name());
}
