#pragma once
#include <iostream>
#include <vector>

enum class EPropertyType { Int, Float, Double, Bool, String };

struct CProperty
{
    std::string Name;
    EPropertyType Type;
    size_t Offset;
};

class GObject {
public:
    virtual ~GObject() = default;

    std::string StaticType() const { return "GObject"; }
    
    virtual std::string GetType() const { return "GObject"; }

    virtual bool IsTypeOf(const std::string& name) const { return StaticType() == name; }

    virtual std::vector<CProperty> GetProperties() const { return {}; }
};

#define GENERATE_REFLECTION(ClassName, ParentClass) \
public: \
std::string GetType() const override { return #ClassName; } \
static std::string StaticType() { return #ClassName; } \
bool IsTypeOf(const std::string& name) const override { \
if (name == #ClassName) return true; \
return ParentClass::IsTypeOf(name); \
} \
virtual std::vector<CProperty> GetProperties() const override { return s_Properties; } \
static void RegisterProperties(CProperty p) { s_Properties.push_back(p); } \
static std::vector<CProperty> s_Properties; \
        
template<class T>
T* Cast(GObject* object)
{
    if (!object)
        return nullptr;

    if (object->IsTypeOf(T::StaticType()))
        return static_cast<T*>(object);

    return nullptr;
}