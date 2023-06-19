#pragma once
#include "Vectors.h"
#include "Schema.h"
class ConVar {
public:
    const char* Name;
    PAD(0x18); // 0x8
    const char* Desc; // 0x20
    uint32_t Type; // 0x28
    uint32_t Registered; // 0x2C
    uint32_t Flags; // 0x30
    PAD(0xC); // 0x34
    uint64_t Value; // 0x40
    uint64_t OldValue; // 0x48

    template <typename T>
    T GetValue() {
        // Tip: Do not modify the value by making this a reference.
        // It'll get your account flagged.
        return *reinterpret_cast<T*>((uintptr_t)(this) + 0x40);
    }
};

class ICvar {
public:
    PAD(0x40)
    utl_linked_list<ConVar*, short> ConVars;
    auto GetFirstCvarIterator(uint64_t& idx) {
        return CallVFunc<void*>(this, 12, &idx);
    }

    auto GetNextCvarIterator(uint64_t& idx) {
        return CallVFunc<void*>(this, 13, &idx, idx);
    }

    auto FindVarByIndex(uint64_t index) {
        return CallVFunc<ConVar*>(this, 37, index);
    }

    auto FindUnsafe(const char* var_name) -> ConVar* {
        // Tip: There's logging in this function because this should run ONLY
        // once for every ConVar. If the console is spammed it means you haven't
        // made the variable static.

        uint64_t i = 0;
        GetFirstCvarIterator(i);
        while (i != 0xFFFFFFFF) {
            ConVar* pCvar = FindVarByIndex(i);
            if (strcmp(pCvar->Name, var_name) == 0) {
             
                return pCvar;
            }
            GetNextCvarIterator(i);
        }


        return nullptr;
    }



    void UnlockAll() noexcept {
        int count = 0;
        for (int i = ConVars.first(); i != ConVars.invalid_index(); i = ConVars.next(i)) {
            auto var = ConVars.element(i);
            var->Flags &= ~0x402;
            count++;
        }
   
    }

    ConVar* Find(const char* name) noexcept {
        for (int i = ConVars.first(); i != ConVars.invalid_index(); i = ConVars.next(i)) {
            auto var = ConVars.element(i);
            if (strcmp(var->Name, name) == 0)
                return var;
        }
        return nullptr;
    }
};
