#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include "component.hpp"
#include "mathlib.hpp"

class Transform : public Component
{
public:
    Transform(EntityId entity_id)
        : Component(entity_id)
    {}

    void Ping() const;
    Matrix transform;
};

#endif // TRANSFORM_HPP_
