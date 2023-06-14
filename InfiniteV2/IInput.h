#pragma once

#include "Signatures.h"
#include "Client.h"
#include "Vectors.h"
#include "UserCmd.h"


enum IInputVTable
{
    CREATEMOVE = 5,
    ON_INPUT = 9,
    MOUSE_INPUT = 10
};

//fix this horrible code once we get the reversed virtual class
class IInput
{
public:
    PAD(0x10); // 0x0
    per_user_input per_user[1]; // 0x10

    UserCmd* GetUserCmd(int split_screen_index) {
        auto input = get_per_user_input(split_screen_index);
        if (!input)
            return nullptr;
        return &input->cmds[input->SequenceNumber % std::size(input->cmds)];
    }
    per_user_input* get_per_user_input(int split_screen_index) {
        if (split_screen_index >= 1) {
            return nullptr;
        }
        return &per_user[split_screen_index];
    }

    void SetViewAngles(Vec3& angles)
    {
        using function_t = std::int64_t(__fastcall*)(IInput*, std::int32_t, Vec3&);
        static function_t fn = reinterpret_cast<function_t>(Client->FindPattern(GetModuleHandleA("client.dll"), SET_VIEW_ANGLES));

        fn(this, 0, angles);
    }
};