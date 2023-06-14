#pragma once
#include "Rendering.h"
#include "Config.h"
enum MenuElementType {

};

class MenuElement {
public:
	virtual MenuElementType GetType() = 0;
	virtual float GetOffset() = 0;
	virtual void Draw(Vec2 Relative, Vec2 Size, float MaxAlpha) = 0;
	virtual bool Update(bool& LeftClick, bool& Drag, bool& disable) = 0;
	virtual bool ShouldRender() = 0;
	virtual bool Overlay() = 0;
};