#pragma once
#include "Rendering.h"
#include "Config.h"


class MenuElement {
public:
	virtual float GetOffset() = 0; //next element offset
	virtual bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) = 0;
	virtual bool ShouldRender() = 0; //should we render this element?
	virtual bool ShouldOverlay() = 0; //should we overlay this element? (rendered last above all other)
//	virtual float& GetAnimation() = 0; //related to open animation
	virtual void OnFree() = 0; //related to element config

};

class Switch : public MenuElement {
public:
	Switch() {
		Slide = 0.f;
		Pointer = nullptr;
		BindedVar = ""; //no bind which is an issue
	}
	Switch(std::string label, bool* val, bool(*shouldrender)() = nullptr) {
		Pointer = val;
		Label = label;
		Slide = 0.f;
		BindedVar = label;
		ShouldRenderFn = shouldrender;
		while (ConfigSystem->VarExists(BindedVar)) {
			BindedVar += "_";
		}
		ConfigSystem->AddVar(BindedVar, val);
	}
	float GetOffset();
	bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable);
	bool ShouldRender();
	bool ShouldOverlay();
	//float& GetAnimation();
	void OnFree();
	float Slide;
	float OffsetAnimation;
	bool(*ShouldRenderFn)();
	std::string Label;
	std::string BindedVar;
	bool* Pointer;
};

class Child {
public:
	Child() {
		Scroll = 0.f;
	}
	Child(std::string title, Vec2 size, Col background, bool outline, bool open) {
		Title = title;
		Size = size;
		Background = background;
		Outline = outline;
		Scroll = 0.f;
		OpenAnimation = 0.f;
		Open = open;
	};
	void Update(std::string title, Vec2 size, Col background, bool outline, bool open = true) {
		Title = title;
		Size = size;
		Background = background;
		Outline = outline;
		Scroll = 0.f;
		OpenAnimation = 0.f;
		Open = open;
	};
	inline bool InRegion(float x, float y, float w, float h);
	bool Open;
	Col Background;
	Vec2 Size;
	bool Outline;
	float Scroll; //from 0-1 
	float OpenAnimation;
	void Draw(float x, float y, float MaxAlpha, bool LeftClick, bool Drag);
	std::string Title;
	void New(MenuElement* Element) {
		Elements.push_back(Element);
	}
	void EraseElements() {
		for (auto& Element : Elements)
			delete Element;


		Elements.clear();
	}
	std::deque< MenuElement* > Elements;
};