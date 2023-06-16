#pragma once
#include "Rendering.h"
#include "Config.h"
#include <map>
#include <string>
namespace GUIAnimations {
	float Ease(const float& a);
	float DelayEase(const float& a, const float& Delay);
	inline void Animate(float& f, bool Enable);
};

class MenuElement {
public:
	virtual float GetOffset() = 0; //next element offset
	virtual bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) = 0;
	virtual bool ShouldRender() = 0; //should we render this element?
	virtual bool ShouldOverlay() = 0; //should we overlay this element? (rendered last above all other)
//	virtual float& GetAnimation() = 0; //related to open animation
	virtual void OnFree() = 0; //related to element config

};

class ConfigView : public MenuElement {
public:
	ConfigView() {
		Reload();
		Scroll = 0.f;
		SelectedConfig = "";
		ToErase.clear();
		 SelectionOpenAnimation = 0.f;
		 SelectionBackAnimation = 0.f;
		 SelectionSaveAnimation = 0.f;
		 SelectionLoadAnimation = 0.f;
		 SelectionCopyAnimation = 0.f;
		 SelectionRenameAnimation = 0.f;
		 SelectionAuthorInfoAnimation = 0.f;
		 SelectionInRename = false;
	}
	struct ConfigAnimation {
		//per config
		float Offset;
		float Delete;
		float Text;

		BaseConfig* Config;
		bool ToBeDeleted;
	};

	float GetOffset();
	bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable);
	bool ShouldRender();
	bool ShouldOverlay();
	void OnFree();
	float Scroll;
	std::string SelectedConfig;
	std::string LastSelectedConfig;
	std::string EditableText;
	std::vector<std::string> ToErase;
	float SelectionOpenAnimation;
	float SelectionBackAnimation;
	float SelectionSaveAnimation;
	float SelectionLoadAnimation;
	float SelectionCopyAnimation;
	float SelectionRenameAnimation;
	float SelectionAuthorInfoAnimation;
	bool SelectionInRename;
	void DrawConfig(float x, float y, float MaxAlpha, std::string label, ConfigAnimation& Context, bool& LeftClick, bool ShouldDoUpdate);
	void Reload() {
		ConfigSystem->Reload();
		for (auto& Cfg : ConfigSystem->Configs) {
			if (ConfigViews.find(Cfg.first) == ConfigViews.end()) {
				ConfigAnimation Config;
				Config.Config = &Cfg.second;
				Config.Delete = 0.f;
				Config.Offset = 0.f;
				Config.Text = 0.f;
				Config.ToBeDeleted = false;
				ConfigViews[Cfg.first] = Config;
			}
			else
				ConfigViews[Cfg.first].Config = &Cfg.second;
		}

		for (auto& View : ConfigViews) {
			if (ConfigSystem->Configs.find(View.first) == ConfigSystem->Configs.end()) {
				View.second.ToBeDeleted = true;
			}
		}
	}

	std::map<std::string, ConfigAnimation> ConfigViews;
};

class Switch : public MenuElement {
public:
	Switch() {
		Slide = 0.f;
		Pointer = nullptr;
		BindedVar = ""; //no bind which is an issue
		HoverAnimation = 0.f;
		OffsetAnimation = 1.f;
	}
	Switch(std::string label, bool* val, bool(*shouldrender)() = nullptr) {
		Pointer = val;
		Label = label;
		Slide = 0.f;
		BindedVar = label;
		HoverAnimation = 0.f;
		ShouldRenderFn = shouldrender;
		while (ConfigSystem->VarExists(BindedVar)) {
			BindedVar += "_";
		}
		ConfigSystem->AddVar(BindedVar, val);
		OffsetAnimation = 1.f;

		if (shouldrender != nullptr)
			OffsetAnimation = shouldrender() ? 1.f : 0.f;
	}
	float GetOffset();
	bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable);
	bool ShouldRender();
	bool ShouldOverlay();
	//float& GetAnimation();
	void OnFree();
	float Slide;
	float HoverAnimation;
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