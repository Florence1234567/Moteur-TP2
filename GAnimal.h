#pragma once
#include "GObject.h"

class GAnimal : public GObject {
    GENERATE_REFLECTION(GAnimal, GObject)

public:
    GAnimal(const std::string& name) {}
    ~GAnimal() = default;
    
    virtual void MakeNoise() const = 0;

    std::string GetName() const { return name; }

    std::string name;
};