#pragma once
#include <iostream>
#include "GAnimal.h"

class GDog : public GAnimal {
    GENERATE_REFLECTION(GDog, GAnimal)

public:
    GDog(const std::string& name) : GAnimal(name) {}
    ~GDog() = default;
    
    void MakeNoise() const override { std::cout << "Woof" << std::endl; }
};