#include "Hooks.h"
#include "Features.h"
std::unique_ptr< VMTHook > Hooks::InputVMTHook = nullptr;

bool __fastcall Hooks::CreateMove(IInput* Input, uint32_t SplitScreenIndex, uint8_t a3) {
    if (!Hooks::oCreateMove) {
        Hooks::oCreateMove = Hooks::InputVMTHook->GetOriginal<Hooks::CreateMove_t>(IInputVTable::CREATEMOVE);
    }

    Client->UpdateLocal(); //should be in FSN

    bool ret = Hooks::oCreateMove(Input, SplitScreenIndex, a3);

 

    auto cmd = Input->GetUserCmd(SplitScreenIndex);
    Client->cmd = cmd;
    if (!cmd || !Client->local)
        return ret;

    Movement::DoBunnyhop();

    return ret;
}