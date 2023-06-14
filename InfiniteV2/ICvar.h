#pragma once
#include "Vectors.h"
#include "Schema.h"
class ConVar {
public:
    const char* Name;

    template <typename T>
    T GetValue() {
        // Tip: Do not modify the value by making this a reference.
        // It'll get your account flagged.
        return *reinterpret_cast<T*>((uintptr_t)(this) + 0x40);
    }
};

class ICvar {
public:
    auto GetFirstCvarIterator(uint64_t& idx) {
        return CallVFunc<void*>(this, 12, &idx);
    }

    auto GetNextCvarIterator(uint64_t& idx) {
        return CallVFunc<void*>(this, 13, &idx, idx);
    }

    auto FindVarByIndex(uint64_t index) {
        return CallVFunc<ConVar*>(this, 37, index);
    }

    auto FindVarByName(const char* var_name) -> ConVar* {
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
};
