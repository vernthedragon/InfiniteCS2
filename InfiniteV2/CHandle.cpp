#include "CHandle.h"

#include "Interfaces.h"

IEntity* CHandle::GetBaseEntity() const {
    if (!g_EntList) return nullptr;

    return g_EntList->GetBaseEntity(GetEntryIndex());
}
