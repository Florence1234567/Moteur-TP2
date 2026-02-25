#pragma once
#include "CComponent.h"
#include "GObject.h"

class CEntity: public GObject
{
    GENERATE_REFLECTION(CEntity, GObject)

public:
    CEntity() = default;
    ~CEntity() = default;

    std::string Name = "Entity";
    std::vector<CComponent*> Components;
};