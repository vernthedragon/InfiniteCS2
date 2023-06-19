#pragma once
#include "Rendering.h"
#include "Config.h"
#include <map>
#include <string>
namespace GUIAnimations {
	float Ease(const float& a);
	float DelayEase(const float& a, const float& Delay);
	inline void Animate(float& f, bool Enable);
	inline void EaseAnimate(float& f, bool Enable, float speed);
};

class MenuElement {
public:
	virtual float GetOffset() = 0; //next element offset
	virtual bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) = 0;
	virtual bool ShouldRender() = 0; //should we render this element?
	virtual bool ShouldOverlay() = 0; //should we overlay this element? (rendered last above all other)
	virtual float GetAnimation() = 0; //related to open animation
	virtual void OnFree() = 0; //related to element config

};

class Child {
public:
	Child() {
		Scroll = 0.f;
	}
	Child(Vec2 size, Col background, bool outline) {

		Size = size;
		Background = background;
		Outline = outline;
		Scroll = 0.f;
		OpenAnimation = 0.f;

	};
	void Update(Vec2 size, Col background, bool outline) {

		Size = size;
		Background = background;
		Outline = outline;
		Scroll = 0.f;
		OpenAnimation = 0.f;
	
	};
	inline bool InRegion(float x, float y, float w, float h);

	Col Background;
	Vec2 Size;
	bool Outline;
	float Scroll; //from 0-1 	
	float AnimatedScroll;
	float OpenAnimation;
	void Draw(float x, float y, float MaxAlpha, bool LeftClick, bool Drag);

	void New(MenuElement* Element) {
		Elements.push_back(Element);
	}

	MenuElement* GetLastAddedElement() {
		return Elements.back();
	}

	std::deque< MenuElement* > Elements;
};
class Bind;
class Switch;
class Select;
class MultiSelect;
class Slider;

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
		 AnimatedScroll = 0.f;
		 SelectionInRename = false;
	}
	struct ConfigAnimation {
		//per config
		float Offset;
		float Delete;
		float Text;
		float Copy;
		BaseConfig* Config;
		bool ToBeDeleted;
	};

	float GetOffset();
	bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable);
	bool ShouldRender();
	bool ShouldOverlay();
	float GetAnimation() {
		return Scroll;
	}
	void OnFree();
	float Scroll;
	float AnimatedScroll;
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
				Config.Copy = 0.f;
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
class Settings : public MenuElement {
public:
	Settings() {

		HoverAnimation = 0.f;
		Open = false;
		OpenAnimation = 0.f;
		HoverAnimation = 0.f;
		Overlay = nullptr;
	}
	Settings(float Offset, float sizex, float sizey, MenuElement* bind, bool(*shouldrender)() = nullptr);
	Settings(float Offset, float sizex, float sizey, MenuElement* bind, void(*Setup)(Child*), bool(*shouldrender)() = nullptr);
	float GetOffset() {
		return 0.f;
	}
	bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable);
	bool ShouldRender() {
		return Before->GetOffset() > 0.f;
	}
	bool ShouldOverlay() {
		return Open;
	}
	float GetAnimation() {
		return HoverAnimation;
	}
	void OnFree() {
		delete Overlay;
	}
	bool SpecialDraw(float MaxAlpha, bool& LeftClick, bool& Drag);
	Vec2 OriginalSize;
	Child* Overlay;
	Vec2 Start;
	MenuElement* Before;
	float offset;
	bool Open;
	float OpenAnimation;
	float HoverAnimation;

};
class ColorPicker : public MenuElement {
public:
	ColorPicker() {

		Pointer = nullptr;
		BindedVar = ""; //no bind which is an issue
		HoverAnimation = 0.f;

	}
	ColorPicker(std::string configvar, float Offset, Col* val, MenuElement* bind) {
		Pointer = val;
		BindedVar = configvar;
		HoverAnimation = 0.f;
		Before = bind;
		int It = 2;
		std::string BindVar = BindedVar;

		while (ConfigSystem->VarExists(BindVar)) {
			BindVar = BindedVar + std::to_string(It);
			It++;
		}
		BindedVar = BindVar;
		ConfigSystem->AddVar(BindedVar, val->Pointer());

		offset = Offset;
		Dialogue.hue = Pointer->Hue();
		Dialogue.saturation = Pointer->Saturation();
		Dialogue.brightness = Pointer->Brightness();
		Dialogue.alpha = Pointer->operator[](3) / 255.f;
	}
	float GetOffset() {
		return 0.f;
	}
	bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable);
	bool ShouldRender();
	bool ShouldOverlay();
	float GetAnimation() {
		return HoverAnimation;
	}
	void OnFree();
	struct ColorPickerDialogue {
		float Open;
		float hue;
		float alpha;
		float saturation;
		Vec2 animatedpicker;
		float animatedfloat;
		float animatedhue;
		float brightness;
		int type;
	};
	ColorPickerDialogue Dialogue;
	struct CopyPasteDialogue {
		float Copy;
		float Paste;
		float Open;
	};
	enum DialogueState_t : std::uint8_t {
		Closed = 0,
		Color = 1,
		CopyPaste = 2
	};
	float offset;
	
	std::uint8_t DialogueState;
	CopyPasteDialogue CPDialogue;
	float HoverAnimation;
	std::string BindedVar;
	Col* Pointer;
	MenuElement* Before;
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

		int It = 2;
		std::string BindVar = BindedVar;

		while (ConfigSystem->VarExists(BindVar)) {
			BindVar = BindedVar + std::to_string(It);
			It++;
		}
		BindedVar = BindVar;
		ConfigSystem->AddVar(BindedVar, val);
		OffsetAnimation = 1.f;

		if (shouldrender != nullptr)
			OffsetAnimation = shouldrender() ? 1.f : 0.f;

		
	}
	float GetOffset();
	bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable);
	bool ShouldRender();
	bool ShouldOverlay();
	float GetAnimation() {
		return OffsetAnimation;
	}
	void OnFree();
	float Slide;
	float HoverAnimation;
	float OffsetAnimation;
	bool(*ShouldRenderFn)();
	std::string Label;
	std::string BindedVar;

	bool* Pointer;
};
class MultiSelect : public MenuElement {
public:
	MultiSelect() {

		Pointer = nullptr;
		BindedVar = ""; //no bind which is an issue
		HoverAnimation = 0.f;
		OffsetAnimation = 0.f;
		Scroll = 0.f;
	}
	MultiSelect(std::string label, const std::vector<std::string>& elements, unsigned int* val, bool(*shouldrender)() = nullptr) {
		Pointer = val;
		Label = label;
		OpenAnimation = 0.f;
		Scroll = 0.f;
		AnimatedScroll = 0.f;
		BindedVar = label;
		HoverAnimation = 0.f;
		ShouldRenderFn = shouldrender;
		Open = false;
		for (auto& Element : elements) {
			Elements.push_back(std::make_pair(0.f, Element));
		}

		int It = 2;
		std::string BindVar = BindedVar;

		while (ConfigSystem->VarExists(BindVar)) {
			BindVar = BindedVar + std::to_string(It);
			It++;
		}
		BindedVar = BindVar;
		ConfigSystem->AddVar(BindedVar, val);

		OffsetAnimation = 1.f;

		if (shouldrender != nullptr)
			OffsetAnimation = shouldrender() ? 1.f : 0.f;
	}
	float GetOffset();
	bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable);
	bool ShouldRender();
	bool ShouldOverlay();
	float GetAnimation() {
		return OffsetAnimation;
	}
	void OnFree();

	float HoverAnimation;
	float OpenAnimation;
	bool Open;
	float OffsetAnimation;
	float Scroll;
	float AnimatedScroll;
	bool(*ShouldRenderFn)();
	std::vector<std::pair<float, std::string>> Elements;
	std::string Label;
	std::string BindedVar;
	unsigned int* Pointer;
};
class Select : public MenuElement {
public:
	Select() {

		Pointer = nullptr;
		BindedVar = ""; //no bind which is an issue
		HoverAnimation = 0.f;
		OffsetAnimation = 0.f;
		Scroll = 0.f;
	}
	Select(std::string label, const std::vector<std::string>& elements, int* val, bool(*shouldrender)() = nullptr) {
		Pointer = val;
		Label = label;
		OpenAnimation = 0.f;
		Scroll = 0.f;
		AnimatedScroll = 0.f;
		BindedVar = label;
		HoverAnimation = 0.f;
		ShouldRenderFn = shouldrender;
		Open = false;
		for (auto& Element : elements) {
			Elements.push_back(std::make_pair(0.f , Element));
		}

		int It = 2;
		std::string BindVar = BindedVar;

		while (ConfigSystem->VarExists(BindVar)) {
			BindVar = BindedVar + std::to_string(It);
			It++;
		}
		BindedVar = BindVar;
		ConfigSystem->AddVar(BindedVar, val);

		OffsetAnimation = 1.f;

		if (shouldrender != nullptr)
			OffsetAnimation = shouldrender() ? 1.f : 0.f;
	}
	float GetOffset();
	bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable);
	bool ShouldRender();
	bool ShouldOverlay();
	float GetAnimation() {
		return OffsetAnimation;
	}
	void OnFree();

	float HoverAnimation;
	float OpenAnimation;
	bool Open;
	float OffsetAnimation;
	float Scroll;
	float AnimatedScroll;
	bool(*ShouldRenderFn)();
	std::vector<std::pair<float, std::string>> Elements;
	std::string Label;
	std::string BindedVar;
	int* Pointer;
};

class Slider : public MenuElement {
public:
	Slider() {
		Pointer = nullptr;
		BindedVar = ""; //no bind which is an issue
		HoverAnimation = 0.f;
		HoverTextAnimation = 0.f;
		OffsetAnimation = 1.f;
		MinMaxDivisor = 0.f;
		OldPointer = 0.f;
	}
	Slider(std::string label, int minvalue, int maxvalue, int* val, bool(*shouldrender)() = nullptr) {
		Pointer = val;
		OldPointer = *val;
		Label = label;
		BindedVar = label;
		HoverAnimation = 0.f;
		ShouldRenderFn = shouldrender;
		HoverTextAnimation = 0.f;
		MinValue = minvalue;
		MaxValue = maxvalue;
		MinMaxDivisor = 1.f / ((float)(abs(MaxValue - MinValue)));
		ModifierX = 0.f;
		int It = 2;
		std::string BindVar = BindedVar;

		while (ConfigSystem->VarExists(BindVar)) {
			BindVar = BindedVar + std::to_string(It);
			It++;
		}
		BindedVar = BindVar;
		ConfigSystem->AddVar(BindedVar, val);
		OffsetAnimation = 1.f;

		if (shouldrender != nullptr)
			OffsetAnimation = shouldrender() ? 1.f : 0.f;
	}
	float GetOffset();
	bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable);
	bool ShouldRender();
	bool ShouldOverlay();
	float GetAnimation() {
		return OffsetAnimation;
	}
	void OnFree();
	float HoverAnimation;
	float OffsetAnimation;
	float HoverTextAnimation;
	float MinMaxDivisor;
	float OldPointer;
	float ModifierX;
	bool TextOpen;
	std::string EditableText;
	bool(*ShouldRenderFn)();
	std::string Label;
	std::string BindedVar;
	int* Pointer;
	int MinValue;
	int MaxValue;
};
class Spacer : public MenuElement {
public:
	Spacer() {

	}
	Spacer(bool(*ShouldRender)()) {

	}
	float GetOffset() {
		return 28.f;
	}
	bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {
		return false;
	}
	bool ShouldRender() {
		return true;
	}
	bool ShouldOverlay() {
		return false;
	}
	float GetAnimation() {
		return 0.f;
	}
	void OnFree() {

	}


};

class Bind : public MenuElement {
public:
	Bind() {
		
	}
	Bind(std::string label, int* vkey) {
		Label = label;
		Pointer = vkey;
		HoverAnimation = 0.f;
	}
	float GetOffset();
	bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable);
	bool ShouldRender();
	bool ShouldOverlay();
	float GetAnimation() {
		return 0.f;
	}
	void OnFree();
	float HoverAnimation;
	std::string Label;
	int* Pointer;

};
class Text : public MenuElement {
public:
	Text() {
		OffsetAnimation = 1.f;
	}
	Text(std::string label, bool(*shouldrender)() = nullptr) {
		OffsetAnimation = 1.f;
		Label = label;
		ShouldRenderFn = shouldrender;
		if (shouldrender != nullptr)
			OffsetAnimation = shouldrender() ? 1.f : 0.f;
	}
	float GetOffset();
	bool Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {
		float OriginalMaxAlpha = MaxAlpha;
		MaxAlpha *= GUIAnimations::Ease(OffsetAnimation);

		Render::DrawString(x, y, Col(255, 255, 255, MaxAlpha), Fonts::MenuThin, 0, Label.c_str());
		return false;
	}
	bool ShouldRender() {
		if (ShouldRenderFn == nullptr)
			return true;

		bool ret = ShouldRenderFn();

		GUIAnimations::EaseAnimate(OffsetAnimation, ret, 0.0183f);


		return ret || OffsetAnimation > 0.f;
	}
	bool ShouldOverlay() {
		return false;
	}
	float GetAnimation() {
		return OffsetAnimation;
	}
	void OnFree() {

	}
	float OffsetAnimation;
	bool(*ShouldRenderFn)();
	std::string Label;

};

class Binder {
public:
	Binder() { Overlay = new Child(Vec2(260.f, 200.f), Col(0, 2, 4, 255), true); }
	~Binder() {}
	void Update(void* parent, const std::string& configvar, std::vector<std::string> el, int Min, int Max, BindParentType type);
	void RenderAndUpdate(float MaxAlpha, bool& LeftClick, bool& Drag);
	void BuildChild();
	void Free();
	ConfigVariable* Var;
	Child* Overlay;
	//child elements
	Bind Keybind;
	Select Type;
	Select OldValue;
	Select NewValue;
	MultiSelect OldValue2;
	MultiSelect NewValue2;
	Slider OldValue3;
	Slider NewValue3;
	Switch ShowBind;
	std::vector<std::string> WritableElements;
	Vec2 Start;
	int MinVal;
	int MaxVal;
	bool Open;
	float OpenAnimation;
	void* Parent;
};