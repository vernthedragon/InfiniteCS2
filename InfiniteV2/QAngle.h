#pragma once
#include "Vectors.h"

class QAngle
{
public:
    char Pad[0x18];
    Vec3 Angles;
};

static_assert(sizeof(QAngle) == 0x24, "QAngle size Wrong"); 
