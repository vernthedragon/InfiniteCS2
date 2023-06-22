#include "Hooks.h"
#include "Features.h"


void __fastcall Hooks::RenderStart(void* a1) {
	//Client->Log("RenderStart\n");
	if(!GameHandler->local || !GameHandler->localcontroller)
		return Hooks::oRenderStart(a1);

	if(!GameHandler->local->IsAlive())
		return Hooks::oRenderStart(a1);

	if (Config->World.Camera) {

		if (GameHandler->local->m_pCameraServices())
			GameHandler->local->m_pCameraServices()->m_iFOV() = Config->World.FOV;
		
	}

	if (Config->World.Enable) {
		//"Fog", "Punch", "Color Effects", "Post Effects", "Smoke" 
		if(Config->World.Removals & 1)
			if (GameHandler->local->m_pCameraServices())
				GameHandler->local->m_pCameraServices()->m_CurrentFog().enable() = false;

		if (Config->World.Removals & 2)
			if (GameHandler->local->m_pCameraServices()) {
				GameHandler->local->m_pCameraServices()->m_vecPunchAngle().angles = Vec3(0, 0, 0);
				GameHandler->local->m_pCameraServices()->m_vecPunchAngleVel().angles = Vec3(0, 0, 0);

				GameHandler->local->m_pCameraServices()->m_nPunchAngleJoltTickClientSide() = INT_MAX;
			}

		if (Config->World.Removals & 4)
			if (GameHandler->local->m_pCameraServices())
				if(GameHandler->local->m_pCameraServices()->m_hColorCorrectionCtrl().Get()) 
					GameHandler->local->m_pCameraServices()->m_hColorCorrectionCtrl().Get<IColorCorrection>()->m_bEnabled() = false;


	}

	Hooks::oRenderStart(a1);
}