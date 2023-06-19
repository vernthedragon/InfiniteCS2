#pragma once

#include <cstdint>
#include "Vectors.h"
#include "QAngle.h"



class CmdVector : public pb_base {
public:
    Vec3 vector; // 0x18
    float w; // 0x24
};

static_assert(sizeof(CmdVector) == 0x28);

class TickInterpolationInfo : public pb_base {
public:
    float frac; // 0x18
    int src_tick; // 0x1C
    int dst_tick; // 0x20
};

static_assert(sizeof(TickInterpolationInfo) == 0x24);

class InputHistoryEntry : public pb_base {
public:
    QAngle* view_angles; // 0x18
    CmdVector* shoot_position; // 0x20
    CmdVector* target_head_pos_check; // 0x28
    CmdVector* target_abs_pos_check; // 0x30
    QAngle* target_abs_ang_check; // 0x38
    TickInterpolationInfo* cl_interp; // 0x40
    TickInterpolationInfo* sv_interp0; // 0x48
    TickInterpolationInfo* sv_interp1; // 0x50 
    TickInterpolationInfo* player_interp; // 0x58
    int render_tick_count; // 0x60
    float render_tick_fraction; // 0x64
    int player_tick_count; // 0x68
    float player_tick_fraction; // 0x6C
    int frame_number; // 0x70
    int target_ent_index; // 0x74
};

class UserCmdBase
{
public:
    char pad1[0x40];
    QAngle* view;
    int commandnumber; // 0x48
    int tickcount; // 0x4C
    float forwardmove; // 0x50
    float sidemove; // 0x54
    float upmove; // 0x58
};


struct UserCmdInButtonState {
    PAD(sizeof(void*)); // 0x0 (vtable)
    uint64_t Button1; // 0x8
    uint64_t Button2; // 0x10
    uint64_t Button3; // 0x18
};
class SubtickCountainer {
public:
    std::int32_t tick_count; // 0x0
    PAD(0x4) // 0x4
    std::uint8_t* TickPointer; // 0x8

    InputHistoryEntry* GetInputHistoryEntry(int32_t index) {
        if (index < this->tick_count)
        {
           
            return GetTicklist()[index];
        }

        return nullptr;
    }
    InputHistoryEntry** GetTicklist() {
        return reinterpret_cast<InputHistoryEntry**>(this->TickPointer + 0x8);
    }
};
class UserCmd
{
public:
    PAD(0x20) // 0x0
    SubtickCountainer Subticks; // 0x20
    UserCmdBase* Base; // 0x30
    int Attack3StartHistoryIndex; // 0x38 (release attack1)
    int Attack1StartHistoryIndex; // 0x3C (press attack1)
    int Attack2StartHistoryIndex; // 0x40 (press attack2)
    PAD(0x4) // 0x44
    UserCmdInButtonState Buttons; // 0x48
    PAD(0x8) // 0x68
    void SetSubTickAngles(Vec3& angles)
    {
        SubtickCountainer container = Subticks;
        for (std::int32_t i = 0; i < container.tick_count; i++)
        {
            InputHistoryEntry* tick = container.GetInputHistoryEntry(i);

            if (tick && tick->view_angles)
            {
                tick->view_angles->angles = angles;
            }
        }
    }
};
struct SubtickMoveStep {
    uint64_t Button;
    bool Pressed;
    float When;
};
struct per_user_input {
    UserCmd cmds[150]; // 0x0
    PAD(0x30) // 0x41A0
        int SequenceNumber; // 0x41D0
    PAD(0x4) // 0x41D4
        uint64_t Button1; // 0x41D8 (is held in this tick)
    uint64_t Button2; // 0x41E0 (has been pressed down in this tick)
    uint64_t Button3; // 0x41E8 (has been released in this tick)
    uint64_t Button4; // 0x41F0 (was held down in previous tick)
    PAD(0x80) // 0x41F8
    float forwardmove;
    float leftmove;
    float upmove;
    Vec3 viewangles; // 0x4284
    int16_t mousedx; // 0x4290
    PAD(0x2) // 0x4292
        int16_t mousedy; // 0x4294
    PAD(0x2) // 0x4296
    int SubtickCount; // 0x4298
    SubtickMoveStep subticks[12]; // 0x429C
};
