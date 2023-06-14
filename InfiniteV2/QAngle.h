#pragma once
#include "Vectors.h"

class pb_base {
private:
    PAD(0x18) // 0x0
};

class QAngle : public pb_base {
public:
    Vec3 angles; // 0x18
};

