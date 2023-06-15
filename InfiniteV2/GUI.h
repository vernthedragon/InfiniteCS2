#pragma once
#include "Rendering.h"
#include "Config.h"


class MenuElement {
public:
	virtual float GetOffset() = 0;
	virtual bool Draw(Vec2 Relative, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) = 0;
	virtual bool ShouldRender() = 0;
	virtual bool ShouldOverlay() = 0;
	virtual float& GetAnimation() = 0;
	virtual void OnReset() = 0;

};


class Child {
public:
	Child() {

	}
	Child(std::string title, Vec2 size, Col background, bool outline) {
		Title = title;
		Size = size;
		Background = background;
		Outline = outline;
	};
	void Update(std::string title, Vec2 size, Col background, bool outline) {
		Title = title;
		Size = size;
		Background = background;
		Outline = outline;
	};
	Col Background;
	Vec2 Size;
	bool Outline;
	void Draw(float x, float y, float MaxAlpha, bool LeftClick, bool Drag);
	std::string Title;
	std::deque< MenuElement* > Elements;
};