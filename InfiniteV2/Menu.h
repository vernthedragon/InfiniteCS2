#pragma once
#include "GUI.h"


class CMenu {
public:
	void Draw();
	void OnRender();
	bool IsHovered();

	float Alpha;
};

extern CMenu* Menu;