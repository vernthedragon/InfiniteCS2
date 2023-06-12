#pragma once
#include <cstdint>
#include <string>

namespace SchemaSystem
{
    //NOTE: IF CRASHED IN SchemaOffset or GetSchema change uint16 to 32 (overflow)
    struct SchemaOffset
    {
        std::string Class;
        std::string Prop;
        std::uint16_t Offset;
    };

    bool Initialize();
    std::uint16_t GetSchema(std::string class_name, std::string property_name);
}

#define SCHEMA(class_name, property_name, function, type) \
__forceinline type& function() \
{ \
    std::uint32_t offset = SchemaSystem::GetSchema(class_name, property_name);  \
    return *reinterpret_cast<type*>(reinterpret_cast<std::uint64_t>(this) + offset); \
} \