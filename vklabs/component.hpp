#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

#include <string>

typedef std::size_t ComponentTypeId;
typedef std::size_t ComponentId;

class Component
{
public:
    ComponentTypeId GetTypeId();

protected:
    ComponentId id_;

};

#endif // COMPONENT_HPP_
