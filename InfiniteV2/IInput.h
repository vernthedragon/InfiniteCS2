#pragma once

#include "Signatures.h"
#include "Client.h"
#include "Vectors.h"
#include "UserCmd.h"


enum IInputVTable
{
    CREATEMOVE = 5,
    MOUSE_INPUT = 10
};

//fix this horrible code once we get the reversed virtual class
class IInput
{
public:
    UserCmd* GetUserCmd(void* a1, std::uint32_t a2)
    {
        // 48 63 DA 4C 8B E1 4C 69 EB
        std::int64_t v5 = 0x460 * a2;
        std::int64_t v6 = *reinterpret_cast<std::int64_t*>(reinterpret_cast<std::uintptr_t>(a1) + v5 + 0x41E0);
        std::int64_t v7 = 0x70 * (0x9A * a2 + v6 % 0x96) + reinterpret_cast<std::uintptr_t>(a1);
        std::uint64_t split_screen = v7 + 0x10;

        return reinterpret_cast<UserCmd*>(split_screen);
    }

    void SetViewAngles(Vec3& angles)
    {
        using function_t = std::int64_t(__fastcall*)(IInput*, std::int32_t, Vec3&);
        static function_t fn = reinterpret_cast<function_t>(Client->FindPattern(GetModuleHandleA("client.dll"), SET_VIEW_ANGLES));

        fn(this, 0, angles);
    }
};