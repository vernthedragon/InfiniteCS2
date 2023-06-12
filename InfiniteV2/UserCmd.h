#pragma once

#include <cstdint>
#include "Vectors.h"
#include "QAngle.h"


class UserCmdBase
{
public:
    char pad1[0x40];
    QAngle* view;
};



class UserCmd
{
public:
    char pad1[0x30];
    UserCmdBase* base;
    char pad2[0x18];
    std::uint32_t buttons;
};

