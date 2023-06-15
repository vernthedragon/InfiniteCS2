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

class Switch : public MenuElement {
public:
	Switch() {
		Slide = 0.f;
	}
	Switch(std::string label, bool* val) {
		Pointer = val;
		Label = label;
		Slide = 0.f;
	}
	float GetOffset() = 0;
	bool Draw(Vec2 Relative, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) = 0;
	bool ShouldRender() = 0;
	bool ShouldOverlay() = 0;
	float& GetAnimation() = 0;
	void OnReset() = 0;
	float Slide;
	std::string Label;
	bool* Pointer;
};

class Child {
public:
	Child() {
		Scroll = 0.f;
	}
	Child(std::string title, Vec2 size, Col background, bool outline) {
		Title = title;
		Size = size;
		Background = background;
		Outline = outline;
		Scroll = 0.f;
	};
	void Update(std::string title, Vec2 size, Col background, bool outline) {
		Title = title;
		Size = size;
		Background = background;
		Outline = outline;
		Scroll = 0.f;
	};
	inline bool InRegion(float x, float y, float w, float h);
	Col Background;
	Vec2 Size;
	bool Outline;
	float Scroll; //from 0-1 
	void Draw(float x, float y, float MaxAlpha, bool LeftClick, bool Drag);
	std::string Title;
	std::deque< MenuElement* > Elements;
};