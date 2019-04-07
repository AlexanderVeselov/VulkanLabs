#include "transform.hpp"
#include "component_manager.hpp"

REGISTER_COMPONENT_CLASS(Transform, transform);

void Transform::Ping() const
{
    printf("Transform::Ping()\n");
}
