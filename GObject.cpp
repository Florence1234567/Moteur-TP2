#include "GObject.h"
#include "GAnimal.h"
#include "GCat.h"
#include "GDog.h"
#include "CEntity.h"
#include "CComponent.h"

std::vector<CProperty> GAnimal::s_Properties;
std::vector<CProperty> GCat::s_Properties;
std::vector<CProperty> GDog::s_Properties;
std::vector<CProperty> CEntity::s_Properties;
std::vector<CProperty> CComponent::s_Properties;