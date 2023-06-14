#include "Menu.h"


void CMenu::Draw() {
	Render::FilledRect(200, 200, 600, 400, Col(3, 14, 9, 255));
}

bool CMenu::IsHovered() {
	return false; //this is temporary
}

void CMenu::OnRender() {

	if (Client->KeyToggled(VK_INSERT)) {
		Config->MenuOpen = !Config->MenuOpen;
	}

	this->Draw();
}

CMenu* Menu = new CMenu();