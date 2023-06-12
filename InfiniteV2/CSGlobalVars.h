#pragma once

#include <cstdint>

class CSGlobalVars
{
public:
    float Realtime;
    std::int32_t Framecount;
    char pad1[0x8];
    std::uint32_t MaxClients;
    float Interval;
    char pad2[0x12];
    float Curtime;
    float Curtime2;
    char pad3[0xC];
    std::int32_t Tickcount;
    float Interval2;
    void* Netchannel;
    char pad4[0x130];
    char* Map;
    char* Mapname;
};