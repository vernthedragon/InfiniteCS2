#include "Rendering.h"


void Render::DoRender(ID3D11Device* Device, ID3D11DeviceContext* Context, HWND Window, ID3D11RenderTargetView* RenderView) {

	Client->UpdateKeyStates();

	if (Client->KeyToggled(VK_INSERT)) {
		Config->MenuOpen = !Config->MenuOpen;
	}
	if (Config->MenuOpen) {
		unsigned char TestColor[4];
		TestColor[0] = 255;
		TestColor[1] = 255;
		TestColor[2] = 255;
		TestColor[3] = 255;

		ImU32 Col = (ImU32)((unsigned long)(*((unsigned long*)TestColor))); //please ignore this
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(300, 300), ImVec2(900, 900), Col);

		//THIS IS ONLY TESTING RENDERING IGNORE THIS HORRENDOUS CODE
	}
}