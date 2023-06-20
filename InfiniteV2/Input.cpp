#include "Hooks.h"
#include "Features.h"


bool __fastcall Hooks::CreateMove(IInput* Input, uint32_t SplitScreenIndex, uint8_t a3) {
    InputVMTHook->GetOriginal(oCreateMove, IInputVTable::CREATEMOVE);

 

    bool ret = oCreateMove(Input, SplitScreenIndex, a3);

 

    auto cmd = Input->GetUserCmd(SplitScreenIndex);
    GameHandler->cmd = cmd;
    if (!cmd || !GameHandler->local || !GameHandler->localcontroller)
        return ret;

    if (!GameHandler->Alive)
        return ret;

    GameHandler->OriginalCmdViewAngles = cmd->Base->view->angles;
    GameHandler->CmdViewAngle = GameHandler->OriginalCmdViewAngles; //set to client viewangle since InputCreateMove is late?
    
    //AntiAim::DoAntiAim(cmd);
    
    Movement::DoMovement(cmd);
    GameHandler->CmdViewAngle.Normalize();
    cmd->Base->view->angles = GameHandler->CmdViewAngle;
    return ret;
}