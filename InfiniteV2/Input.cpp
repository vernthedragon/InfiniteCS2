#include "Hooks.h"
#include "Features.h"
std::unique_ptr< VMTHook > Hooks::InputVMTHook = nullptr;

std::int64_t __stdcall Hooks::SendMove(DWORD* a1) {

    if (AntiAim::ShouldChokePacket())
        return 0;

    return Hooks::oSendMove(a1);
}
bool __fastcall Hooks::CreateMove(IInput* Input, uint32_t SplitScreenIndex, uint8_t a3) {
 //   if (!Hooks::oCreateMove) {
  //      Hooks::oCreateMove = Hooks::InputVMTHook->GetOriginal<Hooks::CreateMove_t>(IInputVTable::CREATEMOVE);
 //   }

    Client->UpdateLocal(); //should be in FSN

    bool ret = Hooks::oCreateMove(Input, SplitScreenIndex, a3);

 

    auto cmd = Input->GetUserCmd(SplitScreenIndex);
    Client->cmd = cmd;
    if (!cmd || !Client->local)
        return ret;

    if (!Client->cache.Alive)
        return ret;

    Client->OriginalViewAngles = cmd->Base->view->angles;
    Client->ViewAngle = Client->ActiveViewAngle; //set to client viewangle since InputCreateMove is late?
    
    AntiAim::DoAntiAim(cmd);
    
    Movement::DoMovement(cmd);
    Client->ViewAngle.Normalize();
    cmd->Base->view->angles = Client->ViewAngle;
    return ret;
}