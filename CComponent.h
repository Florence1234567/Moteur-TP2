#pragma once
#include "GObject.h"

class CComponent : public GObject
{
    GENERATE_REFLECTION(CComponent, GObject)

public:
    CComponent() = default;
    ~CComponent() = default;

    std::string Name = "Component";
};