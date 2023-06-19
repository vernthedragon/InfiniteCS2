#include "GUI.h"
#include "Menu.h"

float GUIAnimations::Ease(const float& a) {
	if (Config->DisableComplexAnimations) {
		return a * a * a;
	}
	if (a == 0.f || a == 1.f) //avoid wasting resources
		return a;

	float sqt = a * a;
	return sqt / (2.0f * (sqt - a) + 1.0f);
	
}
float GUIAnimations::DelayEase(const float& a, const float& Delay) {


	if (a == 1.f)
		return a;

	if (a <= Delay)
		return 0.f;

	return ((a - Delay) / (1.f - Delay));
}

inline void GUIAnimations::Animate(float& f, bool Enable) {
	if ((Enable && f < 1.f) || (!Enable && f > 0.f))
		f = Math::Clamp(f + ((Enable ? 1.f : -1.f) * 0.007f * Menu->AnimationModifier), 0.f, 1.f);
}
inline void GUIAnimations::EaseAnimate(float& f, bool Enable, float speed) {

	if (Config->DisableComplexAnimations) {
		Animate(f, Enable);
	}

	f = (f + ((Enable ? 1.f : 0.f) - f) * speed * Menu->AnimationModifier);
	if (f > 0.993f)
		f = 1.f;

	if (f < 0.013f)
		f = 0.f;
}


#pragma region GUIClass
void Child::Draw(float x, float y, float MaxAlpha, bool LeftClick, bool Drag) {

	//if ((Open && OpenAnimation < 1.f) || (!Open && OpenAnimation > 0.f))
	//	OpenAnimation = Math::Clamp(OpenAnimation + ((Open ? 1.f : -1.f) * 0.0053f * Menu->AnimationModifier), 0.f, 1.f);
	GUIAnimations::EaseAnimate(OpenAnimation, true, 0.01686f);
	MaxAlpha *= OpenAnimation;
	Background[3] = MaxAlpha;
		//NOTE THIS->SIZE IS SCALED IN MENU.CPP
	Render::FilledRoundedRect(x, y, Size.x, Size.y, Background, 4.5f * Menu->Scale);


	
	Render::PushClipRect(x,y, Size.x, Size.y, true);
	
	y -= AnimatedScroll;
	MenuElement* Overlay = nullptr;
	float OverlayContextStartY = 0.f;
	float StartY = 0.f;// + 15.f * Menu->Scale;

	for (auto& Element : this->Elements) {
		
		if (Element->ShouldOverlay()) {
			Overlay = Element;
			break;
		}

		


	}

	bool Disable = false;
	bool OldMouseClick = Menu->MouseClick;
	bool OldMousePress = Menu->MousePress;
	if (Overlay) {
		Menu->MouseClick = false;
		Menu->MousePress = false;
		Disable = true;
	}
	if (Menu->Binder.Parent != nullptr && Menu->Binder.Overlay != this)
		Disable = true;
 	for (auto& Element : this->Elements) {

	
		StartY += Element->GetOffset();

		if (Element->ShouldRender())
		{
			if (Element == Overlay) { 
				OverlayContextStartY = y - 13.f * Menu->Scale + StartY;
				continue;
			}
			Element->Draw(x + 15.f * Menu->Scale, y - 13.f * Menu->Scale + StartY, Size, MaxAlpha, Menu->MouseClick, Menu->MousePress, Disable);
		}
		
		//StartY += Element->GetOffset();
	}
	y += AnimatedScroll;

	bool ShouldScroll = StartY > Size.y - 13.f * Menu->Scale;
	if (ShouldScroll) {
		bool Scrolled = false;
		while (Client->ScrollAmmount > 0) {
			Scroll -= 19.f * Menu->Scale;
			Client->ScrollAmmount--;
			Scrolled = true;
		}
		while (Client->ScrollAmmount < 0) {
			Scroll += 19.f * Menu->Scale;
			Client->ScrollAmmount++;
			Scrolled = true;
		}
		float Max = (StartY) - (15.f * Menu->Scale * 28.f) - Menu->Scale * 3.f;
		Math::ClampPtr(Scroll, Scrolled ? -140.f * Menu->Scale : 0.f, Scrolled ? (Max + 140.f * Menu->Scale) : Max);
		AnimatedScroll = (AnimatedScroll + (Scroll - AnimatedScroll) * 0.02f * Menu->AnimationModifier);

		
		float OffsetY = (AnimatedScroll / Max) * (Size.y - 40.f * Menu->Scale);
		Render::FilledRoundedRectCustom(x + Size.x - 4.f * Menu->Scale, y + OffsetY, 4.f * Menu->Scale, 40.f * Menu->Scale, Col(0, 3, 6, MaxAlpha), 4.5f * Menu->Scale, ImDrawFlags_::ImDrawFlags_RoundCornersRight);
		
	}

	Menu->MouseClick = OldMouseClick;
	Menu->MousePress = OldMousePress;
	Render::PopClipRect();

	if (Overlay != nullptr) {
		Overlay->Draw(x + 15.f * Menu->Scale, OverlayContextStartY, Size, MaxAlpha, Menu->MouseClick, Menu->MousePress, Disable);
	}
}

inline bool Child::InRegion(float x, float y, float w, float h) {
	return Menu->MousePos.x > x && Menu->MousePos.y > y && Menu->MousePos.x < x + w && Menu->MousePos.y < y + h;
}
#pragma endregion 
bool Settings::SpecialDraw(float MaxAlpha, bool& LeftClick, bool& Drag) {
	if (OpenAnimation <= 0.f)
		return false;

	MaxAlpha *= GUIAnimations::Ease(OpenAnimation);
	
	Overlay->Size = OriginalSize;
	Overlay->Size.x *= Menu->Scale;
	Overlay->Size.y *= Menu->Scale;
	Overlay->Draw(Start.x, Start.y + 36.f * (1.f - OpenAnimation), MaxAlpha, LeftClick, Drag);

	if (Open && LeftClick && Menu->Binder.Parent == nullptr) {
		if (!Menu->InRegion(Start.x, Start.y, OriginalSize.x * Menu->Scale, OriginalSize.y * Menu->Scale))
		{
			Open = false;
		}
	}

	return true;
}
Settings::Settings(float Offset, float sizex, float sizey, MenuElement* bind, bool(*shouldrender)() ) {

	HoverAnimation = 0.f;

	Before = bind;


	Open = false;
	OpenAnimation = 0.f;
	HoverAnimation = 0.f;

	offset = Offset;
	OriginalSize = Vec2(sizex, sizey);
	Overlay = new Child(Vec2(sizex, sizey), Col(0, 2, 4, 255), true);
	Menu->SettingsWindows.push_back(this);
}
void Binder::Update(void* parent, const std::string& configvar, std::vector<std::string> el, int Min, int Max, BindParentType type) {
	Var = ConfigSystem->BindBindWithVar(configvar, type);
	if (!Var)
		return;
	MaxVal = Max;
	MinVal = Min;
	WritableElements.clear();
	WritableElements = el;
	BuildChild();
	Parent = parent;
}
void Binder::Free() {
	
	/*
	if (Keybind)
		delete Keybind;

	if (Type)
		delete Type;

	if (NewValue)
		delete NewValue;
	if (OldValue)
		delete OldValue;

	if (OldValue2)
		delete OldValue2;
	if (NewValue2)
		delete NewValue2;

	if (OldValue3)
		delete OldValue3;
	if (NewValue3)
		delete NewValue3;

	
	Keybind = nullptr;
	Type = nullptr;
	OldValue = nullptr;
	NewValue = nullptr;
	OldValue2 = nullptr;
	NewValue2 = nullptr;
	OldValue3 = nullptr;
	NewValue3 = nullptr;*/
}
void Binder::BuildChild() {
	if(!Overlay)
		Overlay = new Child(Vec2(260.f, 200.f), Col(0, 2, 4, 255), true);

	Overlay->Elements.clear();
	Keybind =  Bind("Binded Key", &Var->Bind.VKey);
	Overlay->New(&Keybind);

	if (WritableElements.size() != 0 && Var->Bind.ParentType == BindParentType::BindtypeSlider)
		Var->Bind.ParentType = BindParentType::BindtypeSelect;

	if (Var->Bind.ParentType == BindParentType::BindtypeSlider) {
		OldValue3 =  Slider("Bind Off Value", MinVal, MaxVal, &(((SliderBind*)Var->Bind.Data)->OldValue));
		NewValue3 =   Slider("Bind On Value", MinVal, MaxVal, &(((SliderBind*)Var->Bind.Data)->NewValue));
		Overlay->New(&OldValue3);
		Overlay->New(&NewValue3);
	}
	else if(Var->Bind.ParentType == BindParentType::BindtypeSelect) {
		OldValue =  Select("Bind Off Value",  WritableElements, &(((SliderBind*)Var->Bind.Data)->OldValue));
		NewValue =  Select("Bind On Value", WritableElements, &(((SliderBind*)Var->Bind.Data)->NewValue));
		Overlay->New(&OldValue);
		Overlay->New(&NewValue);
	}
	else if (Var->Bind.ParentType == BindParentType::BindtypeMultiselect) {
		OldValue2 =  MultiSelect("Bind Off Value", WritableElements, &(((MultiSelectBind*)Var->Bind.Data)->OldValue));
		NewValue2 =  MultiSelect("Bind On Value", WritableElements, &(((MultiSelectBind*)Var->Bind.Data)->NewValue));
		Overlay->New(&OldValue2);
		Overlay->New(&NewValue2);
	}
	Type =  Select("Bind Type", { "Disabled", "Hold", "Toggle", "Release" }, &Var->Bind.Type);
	Overlay->New(&Type);
	ShowBind = Switch("Show In Keybinds", &Var->Bind.Show);
	Overlay->New(&ShowBind);
}
void Binder::RenderAndUpdate(float MaxAlpha, bool& LeftClick, bool& Drag) {

	if (!Var)
		return;

	if (MaxAlpha != 255.f)
		Open = false;
	MaxAlpha *= GUIAnimations::Ease(OpenAnimation);

	GUIAnimations::EaseAnimate(OpenAnimation, Open, 0.0178f);

	if (OpenAnimation <= 0.f) {
		return;
	}



	MaxAlpha *= GUIAnimations::Ease(OpenAnimation);

	Overlay->Size = Vec2(Var->Bind.ParentType == BindtypeSlider ? 350.f * Menu->Scale : 260.f * Menu->Scale, Var->Bind.ParentType == BindtypeSwitch ? 102.f * Menu->Scale : 158.f * Menu->Scale);

	Overlay->Draw(Start.x, Start.y + 36.f * (1.f - OpenAnimation), MaxAlpha, LeftClick, Drag);
	if (LeftClick) {
		if (LeftClick && !Menu->InRegion(Start.x, Start.y, Overlay->Size.x, Overlay->Size.y))
		{
			Open = false;
			LeftClick = false;
		}
	}
}
float Text::GetOffset() {
	return OffsetAnimation * Menu->Scale * 28.f;
}
float Bind::GetOffset() {
	return Menu->Scale * 28.f;
}
const std::vector<std::string> KeyStrings2 = {
	"-", "Mouse 1", "Mouse 2", "cb", "Mouse 3", "Mouse 4", "Mouse 5",
	"?", "Back", "Tab", "?", "?", "?", "Enter", "?", "?", "Shift", "Ctrl", "Alt", "?",
	"Caps", "?", "?", "?", "?", "?", "?", "Escape", "?", "?", "?", "?", "Space",
	"?", "?", "End", "Home", "Left", "Up", "Right", "Down", "?", "?", "?", "?", "Ins",
	"Delete", "?", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "?", "?", "?", "?", "?", "?",
	"?", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
	"y", "z", "?", "?", "?", "?", "?", "n0", "n1", "n2", "n3", "n4", "n5", "n6",
	"n7", "n8", "n9", "*", "+", "_", "-", ".", "/", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12",
	"f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "Shift", "Shift", "Ctrl", "Ctrl", "Menu", "Menu", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", ";", "+", ",", "-", ".", "/?", "~", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "[{", "\\|", "}]", "'\"", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?","?", "?", "?"
};
bool Bind::Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {
	float OriginalMaxAlpha = MaxAlpha;


	if (MaxAlpha != 255.f && *Pointer < 0) {
		if (*Pointer == -2) {
			if (Config->AutoSave)
				ConfigSystem->SaveToConfig(ConfigSystem->Loaded);
		}
		*Pointer = -1;
	}
	bool bHovered = Menu->InRegion(x + Size.x - 123.f * Menu->Scale , y - 10.f * Menu->Scale, 84.f * Menu->Scale, 23.f * Menu->Scale) && !disable ;
	GUIAnimations::Animate(HoverAnimation, bHovered || *Pointer == -2);
	Render::DrawString(x, y, Col(255, 255, 255, MaxAlpha), Fonts::MenuThin, 0, Label.c_str());
	std::string Written = "-";

	if (*Pointer == -2)
		Written = "...";
	else if (*Pointer > 0) {
		if (*Pointer < KeyStrings2.size())
			Written = KeyStrings2[*Pointer];
		else
			Written = "?";
	}

	Render::FilledRoundedRect(x + Size.x - 99.f * Menu->Scale, y - 4.f * Menu->Scale, 72.f * Menu->Scale, 24.f * Menu->Scale, Col(0, 0, 1, MaxAlpha * (0.36f + 0.5f * HoverAnimation)), 3.f * Menu->Scale);
	Render::RoundedRect(x + Size.x - 99.f * Menu->Scale, y - 4.f * Menu->Scale, 72.f * Menu->Scale, 24.f * Menu->Scale, Col(100 + 155 * HoverAnimation, 104 + 151 * HoverAnimation, 110 + 145 * HoverAnimation, MaxAlpha * (0.36f + 0.5f * HoverAnimation)), 0.9f * Menu->Scale, 3.f * Menu->Scale);
	Render::PushClipRect(x + Size.x - 99.f * Menu->Scale, y - 4.f * Menu->Scale, 72.f * Menu->Scale, 24.f * Menu->Scale, true);
	Render::DrawString(x + Size.x - 62.5f * Menu->Scale, y - 1.5f * Menu->Scale, Col(255, 255, 255, MaxAlpha), Fonts::MenuThin, Render::centered_x, Written.c_str());
	Render::PopClipRect();

	if (bHovered && LeftClick && *Pointer != -2) {
		*Pointer = -2;

		
	}
	else if (*Pointer == -2) {
		for (auto i = 0; i < 256; i++) {
			if (Client->KeyToggled(i)) {
				if (i == VK_ESCAPE) {
					*Pointer = -1;
					if (Config->AutoSave)
						ConfigSystem->SaveToConfig(ConfigSystem->Loaded);
				}
				else {
					*Pointer = i;
					if (Config->AutoSave)
						ConfigSystem->SaveToConfig(ConfigSystem->Loaded);
				}
				
			}
		}

	}
	return false;
}
bool Bind::ShouldRender() {
	

	return true;
}
bool Bind::ShouldOverlay() {
	return false;
}
void Bind::OnFree() {

}
Settings::Settings(float Offset, float sizex, float sizey, MenuElement* bind, void(*Setup)(Child*), bool(*shouldrender)() ) {

	HoverAnimation = 0.f;

	Before = bind;


	Open = false;
	OpenAnimation = 0.f;
	HoverAnimation = 0.f;

	offset = Offset;
	OriginalSize = Vec2(sizex, sizey);
	Overlay = new Child(Vec2(sizex, sizey), Col(0, 2, 4, 255), true);
	Setup(Overlay);
	Menu->SettingsWindows.push_back(this);
}
bool Settings::Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {
	float OriginalMaxAlpha = MaxAlpha;
	MaxAlpha *= GUIAnimations::Ease(Before->GetAnimation());

	bool ButtonHovered = Menu->InRegion(x + Size.x - (offset + 8.f) * Menu->Scale, y - 2.f * Menu->Scale, 30.f * Menu->Scale, 19.f * Menu->Scale) && !disable;
	GUIAnimations::Animate(HoverAnimation, ButtonHovered || ShouldOverlay());

	Render::DrawString(x + Size.x - offset * Menu->Scale, y + 7.f * Menu->Scale, Col(90 + HoverAnimation * 165, 90 + HoverAnimation * 165, 90 + HoverAnimation * 165, MaxAlpha * (0.8f + 0.2f * HoverAnimation)), Fonts::MenuIcons, Render::centered_y, "I");

	if (MaxAlpha < 254.8f)
		Open = false;

	GUIAnimations::EaseAnimate(OpenAnimation, Open, 0.0178f);

	if (ButtonHovered && LeftClick) {
		Open = true;
		Start = Menu->MousePos;
	}
	
	return false;
	
}
bool ColorPicker::Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {
	float OriginalMaxAlpha = MaxAlpha;
	MaxAlpha *= GUIAnimations::Ease(Before->GetAnimation());
	if (DialogueState == DialogueState_t::Closed) {
		Dialogue.hue = Pointer->Hue();
		Dialogue.saturation = Pointer->Saturation();
		Dialogue.brightness = Pointer->Brightness();
		Dialogue.alpha = Pointer->operator[](3) * 0.00392156862f;
	}
	bool ButtonHovered = Menu->InRegion(x + Size.x - (offset + 8.f) * Menu->Scale, y - 2.f * Menu->Scale, 30.f * Menu->Scale, 19.f * Menu->Scale) && !disable;
	GUIAnimations::Animate(HoverAnimation, ButtonHovered || ShouldOverlay());
	Col col = *Pointer;
	Render::DrawString(x + Size.x - offset * Menu->Scale, y + 9.f * Menu->Scale, Col(col[0],col[1],col[2], MaxAlpha * (0.8f + 0.2f * HoverAnimation)), Fonts::MenuIcons, Render::centered_y, "U");
	
	if(MaxAlpha < 254.8f)
		DialogueState = DialogueState_t::Closed;

	if (ButtonHovered) {
		if (LeftClick && CPDialogue.Open == 0.f) {
			DialogueState = DialogueState_t::Color;
			Menu->MouseRightClick = false;
			LeftClick = false;
		}
		else if (Menu->MouseRightClick && Dialogue.Open == 0.f) {
			Menu->MouseRightClick = false;
			LeftClick = false;
			DialogueState = DialogueState_t::CopyPaste;
			Dialogue.hue = Pointer->Hue();
			Dialogue.saturation = Pointer->Saturation();
			Dialogue.brightness = Pointer->Brightness();
			Dialogue.alpha = Pointer->operator[](3) / 255.f;
			Dialogue.animatedfloat = (Dialogue.hue);
			Dialogue.animatedpicker = Vec2(140.f * Dialogue.saturation * Menu->Scale, 140.f * (1.f - Dialogue.brightness) * Menu->Scale);
			Dialogue.animatedhue = (Dialogue.animatedhue);
		}
	}
	GUIAnimations::EaseAnimate(CPDialogue.Open, DialogueState == DialogueState_t::CopyPaste, 0.0178f);
	GUIAnimations::EaseAnimate(Dialogue.Open, DialogueState == DialogueState_t::Color, 0.0178f);

	if (CPDialogue.Open > 0.f) {
		float Eased = GUIAnimations::Ease(CPDialogue.Open);

		x = x + Size.x - (offset - 7.f) * Menu->Scale;
		y = y + 25.f * Menu->Scale + (1.f - CPDialogue.Open) * 26.f * Menu->Scale;
	
		Render::FilledRoundedRect(x, y, 79.f * Menu->Scale, 43.f * Menu->Scale, Col(0, 3, 6, MaxAlpha * Eased), 4.f * Menu->Scale);
		Render::DrawString(x + 24.f * Menu->Scale, y + 20.f * Menu->Scale, Col(55 + CPDialogue.Copy * 200, 55 + CPDialogue.Copy * 200, 55 + CPDialogue.Copy * 200, MaxAlpha * Eased * (0.8f + 0.2f * CPDialogue.Copy)), Fonts::MenuIcons, Render::centered_xy, "S");
		bool CopyHovered = Menu->InRegion(x, y, 39.5f * Menu->Scale, 43.f * Menu->Scale);
		bool PasteHovered = Menu->InRegion(x + 40.f * Menu->Scale, y, 39.f * Menu->Scale, 43.f * Menu->Scale);
		Render::DrawString(x + 57.f * Menu->Scale, y + 20.f * Menu->Scale, Col(55 + CPDialogue.Paste * 200, 55 + CPDialogue.Paste * 200, 55 + CPDialogue.Paste * 200, MaxAlpha * Eased * (0.8f + 0.2f * CPDialogue.Paste)), Fonts::MenuIcons, Render::centered_xy, "Z");
		GUIAnimations::Animate(CPDialogue.Paste, PasteHovered);
		GUIAnimations::Animate(CPDialogue.Copy, CopyHovered);

	

		if (DialogueState == DialogueState_t::CopyPaste) {

	
			if (LeftClick ) {
				DialogueState = DialogueState_t::Closed;
				if (CopyHovered) 
					Menu->LastCopiedColor = *Pointer;
		
				
				if (PasteHovered) 
					*Pointer = Menu->LastCopiedColor;
				LeftClick = false;
				
			}
	
		}
	}
	if (Dialogue.Open > 0.f) {
		float Eased = GUIAnimations::Ease(Dialogue.Open);
		float ME = MaxAlpha * Eased;

		x = x + Size.x - (offset + 170.f) * Menu->Scale;

		y = y + 25.f * Menu->Scale + (1.f - Dialogue.Open) * 26.f * Menu->Scale;

		bool Hovered = Menu->InRegion(x, y, 210.f * Menu->Scale, 210.f * Menu->Scale);

		Render::FilledRoundedRect(x, y, 210.f * Menu->Scale, 210.f * Menu->Scale, Col(0, 3, 6, ME), 5.f * Menu->Scale);
		Col picker = Col::hsb(Dialogue.hue, 1.f, 1.f);
		picker[3] = ME;
		Render::GradientFilledRect(x + 20.f * Menu->Scale, y + 20.f * Menu->Scale, 140.f * Menu->Scale, 140.f * Menu->Scale,
			Col(255, 255, 255, ME), picker,
			Col(255, 255, 255, ME), picker);

		Render::GradientFilledRect(x + 20.f * Menu->Scale, y + 20.f * Menu->Scale, 140.f * Menu->Scale, 140.f * Menu->Scale,
			Col(0, 0, 0, 0), Col(0, 0, 0, 0),
			Col(0, 0, 0, ME), Col(0, 0, 0, ME));

		bool HoveredPicker = Menu->InRegion(x + 20.f * Menu->Scale, y + 20.f * Menu->Scale, 140.f * Menu->Scale, 140.f * Menu->Scale);

		Vec2 ToPicker(140.f * Dialogue.saturation * Menu->Scale, 140.f * (1.f - Dialogue.brightness) * Menu->Scale);
		Dialogue.animatedpicker = (Dialogue.animatedpicker + (ToPicker - Dialogue.animatedpicker) * 0.02f * Menu->AnimationModifier);
		Math::ClampPtr(Dialogue.animatedpicker.x, 0.f * Menu->Scale, (140.f-4.f) * Menu->Scale);
		Math::ClampPtr(Dialogue.animatedpicker.y, 0.f * Menu->Scale, (140.f - 4.f) * Menu->Scale);

		Render::FilledCircle(x + 22.f * Menu->Scale + Dialogue.animatedpicker.x, y + 22.f + Dialogue.animatedpicker.y, 4.f * Menu->Scale,
			Col(143, 150, 255, ME), 24);
		Render::FilledCircle(x + 22.f * Menu->Scale + Dialogue.animatedpicker.x, y + 22.f + Dialogue.animatedpicker.y, 3.f * Menu->Scale,
			Col(255, 255, 255, ME), 24);

		constexpr int iterations = 25;
		Col last_color = Col(255, 0, 0, ME);
		const auto& hue_size = Vec2(15.f * Menu->Scale, 140.f * Menu->Scale);
		const auto& hue_pos = Vec2(x + 175.5f * Menu->Scale, y + 50 * Menu->Scale);
		for (auto i = 0; i < iterations; i++) {
			Col current_color = Col::hsb((float)i / iterations, 1.f, 1.f);
			current_color[3] = last_color[3];
			Render::GradientFilledRect(
				hue_pos.x, hue_pos.y + i * hue_size.y / iterations,
				hue_size.x, hue_size.y / iterations,
				last_color, last_color,
				current_color, current_color
			);

			last_color = current_color;
		}
		Dialogue.animatedhue = (Dialogue.animatedhue + (Dialogue.hue - Dialogue.animatedhue) * 0.02f * Menu->AnimationModifier);
		Math::ClampPtr(Dialogue.animatedhue, 0.f, 1.f);
		bool HoveredHue = Menu->InRegion(hue_pos.x, hue_pos.y, hue_size.x, hue_size.y);
		Render::FilledRect(hue_pos.x - 2.f * Menu->Scale, hue_pos.y + hue_size.y * Dialogue.animatedhue, 18.f * Menu->Scale, 4.f * Menu->Scale,
			Col(255, 255, 255, ME));

		Col colactual = col;
		colactual[3] = ME;
		Col colnoa = colactual;
		colnoa[3] = 0.f;
		Render::GradientFilledRect(x + 20.f * Menu->Scale, y + 175.f * Menu->Scale, hue_size.y, hue_size.x,
			colactual, colnoa,
			colactual, colnoa);

		Dialogue.animatedfloat = (Dialogue.animatedfloat + (Dialogue.alpha - Dialogue.animatedfloat) * 0.02f * Menu->AnimationModifier);
		Math::ClampPtr(Dialogue.animatedfloat, 0.f, 1.f);
		Render::FilledRect(x + 20.f * Menu->Scale + hue_size.y * (1.f - Dialogue.animatedfloat), y + 173.f * Menu->Scale, 4.f * Menu->Scale, 18.f * Menu->Scale,
			Col(255, 255, 255, ME));
		bool HoveredAlpha = Menu->InRegion(x + 20.f * Menu->Scale, y + 175.f * Menu->Scale, hue_size.y, hue_size.x);
		colactual[3] = col[3] * (ME * 0.00392156862f);
		Render::FilledRect(x + 173.f * Menu->Scale, y + 20.f * Menu->Scale, 18.5f * Menu->Scale, 15.f * Menu->Scale,
			colactual);

		if (DialogueState == DialogueState_t::Color) {

			if (LeftClick) {
				if (HoveredPicker) {
					Dialogue.type = 1;
				}
				else if (HoveredHue) {
					Dialogue.type = 2;
				}
				else if (HoveredAlpha) {
					Dialogue.type = 3;
				}
				else
				{
					Dialogue.type = 0;
					if (Config->AutoSave)
						ConfigSystem->SaveToConfig(ConfigSystem->Loaded);
				}
				
			
			}
			if (Drag || LeftClick) {
				if (Dialogue.type == 1) {
					float DifferenceX = Menu->MousePos.x - (x + 20.f * Menu->Scale);
					float DifferenceY = Menu->MousePos.y - (y + 20.f * Menu->Scale);

					Dialogue.saturation = std::clamp(DifferenceX / (140.f * Menu->Scale), 0.f, 1.f);
					Dialogue.brightness = std::clamp(1.f - (DifferenceY / (140.f * Menu->Scale)), 0.f, 1.f);
				}
				else if (Dialogue.type == 2) {
					Dialogue.hue = std::clamp((Menu->MousePos.y - (hue_pos.y)) / 140.f * Menu->Scale, 0.f, 1.f);
				}
				else if (Dialogue.type == 3) {
					Dialogue.alpha = std::clamp(1.f - ((Menu->MousePos.x - (x + 20.f * Menu->Scale)) / hue_size.y), 0.f, 1.f);
				}
				*(Col*)Pointer = Col::hsb(Dialogue.hue, Dialogue.saturation, Dialogue.brightness);
				Pointer->operator[](3) = Dialogue.alpha * 255.f;

				if (Drag)
					Drag = false;

				
			}
			else if (Dialogue.type != 0) {
				Dialogue.type = 0;
				if (Config->AutoSave)
					ConfigSystem->SaveToConfig(ConfigSystem->Loaded);
			}

			if (LeftClick && !Hovered) {
				DialogueState = DialogueState_t::Closed;
				

			
			}

			if (LeftClick)
				LeftClick = false;
		}
	}

	return false;
}
bool ColorPicker::ShouldRender() {
	

	return Before->GetOffset() > 0.f;
}
bool ColorPicker::ShouldOverlay() {
	return DialogueState != DialogueState_t::Closed;
}

void ColorPicker::OnFree() {
	if (BindedVar == "") {
		throw IException("Menu Element has no Binded ConfigVar! (Memory Range Corrupt)", 0);
	}

	ConfigSystem->RemoveVar(BindedVar);
}

float Switch::GetOffset() {
	return OffsetAnimation * Menu->Scale * 28.f;
}
bool Switch::Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {
	float OriginalMaxAlpha = MaxAlpha;
	MaxAlpha *= GUIAnimations::Ease(OffsetAnimation);

	bool Hovered = Menu->InRegion(x + Size.x - 76.f * Menu->Scale, y - 6.f * Menu->Scale, 47.f * Menu->Scale, 30.f * Menu->Scale) && !disable && OffsetAnimation == 1.f;

	if ((Hovered && HoverAnimation < 1.f) || (!Hovered && HoverAnimation > 0.f))
		HoverAnimation = Math::Clamp(HoverAnimation + ((Hovered ? 1.f : -1.f) * 0.006f * Menu->AnimationModifier), 0.f, 1.f);


	//Slide = (Slide + ((*Pointer ? 1.f : 0.f) - Slide) * 0.003f * Menu->AnimationModifier);
	GUIAnimations::EaseAnimate(Slide, *Pointer, 0.0178f);
	float EasedSlide = Slide;
	float W = 120 + 135 * EasedSlide; //slight optimization for more memory usage lol
	if (Hovered && LeftClick) {
		LeftClick = false;
		disable = true;
		*Pointer = !(*Pointer);

		if(Config->AutoSave)
			ConfigSystem->SaveToConfig(ConfigSystem->Loaded);
	}
	else if (Hovered && Menu->MouseRightClick) {
		if (Menu->Binder.Parent == nullptr && Label != "Show In Keybinds") {
			Menu->Binder.Update(this, BindedVar, {}, 0, 0, BindtypeSwitch);
			Menu->MouseRightClick = false;
			Menu->Binder.Open = true;
			Menu->Binder.Start = Menu->MousePos;
		}
	}
	
		
	
	/*
	if (Render::TextSize(Fonts::MenuThin, Label.c_str()).x > Size.x - 103.f * Menu->Scale) {
		Render::PushClipRect(x, y - 4.f * Menu->Scale, Size.x - 103.f * Menu->Scale, 23.f * Menu->Scale, true);
		Render::DrawString(x, y, Col(W, W, W, MaxAlpha), Fonts::MenuThin, 0, Label.c_str());
		Col Full = Col(0, 1, 2, OriginalMaxAlpha);
		Col Low = Col(0, 1, 2, 0);
		//note: rendered twice for quadratic effect
		Render::GradientFilledRect(x + Size.x - 133.f * Menu->Scale, y - 4.f * Menu->Scale, 33.f * Menu->Scale, 26.f * Menu->Scale, Low, Full, Low, Full);
		Render::GradientFilledRect(x + Size.x - 133.f * Menu->Scale, y - 4.f * Menu->Scale, 33.f * Menu->Scale, 26.f * Menu->Scale, Low, Full, Low, Full);
		Render::GradientFilledRect(x + Size.x - 133.f * Menu->Scale, y - 4.f * Menu->Scale, 33.f * Menu->Scale, 26.f * Menu->Scale, Low, Full, Low, Full);
		Render::PopClipRect();
	}
	else*/
		Render::DrawString(x, y, Col(W, W, W, MaxAlpha), Fonts::MenuThin, 0, Label.c_str());

	Col Main(11 + 132 * EasedSlide, 12 + 138 * EasedSlide, 18 + 237 * EasedSlide, MaxAlpha);
	Col Background(0, 3, 6, MaxAlpha * 0.78f);	
	Render::FilledRoundedRect(x + Size.x - 69.f * Menu->Scale, y, 39.f * Menu->Scale, 16.f * Menu->Scale, Background, 20.f * Menu->Scale);
	if (HoverAnimation > 0) {
		Render::GradientCircle(x + Size.x - (62.5f - 25.f * EasedSlide * (OriginalMaxAlpha * 0.003922f)) * Menu->Scale, y + 8.5f * Menu->Scale, 17.f * Menu->Scale, Col(11 + 132 * EasedSlide * 0.43f, 12 + 138 * EasedSlide * 0.43f, 18 + 237 * EasedSlide * 0.43f, MaxAlpha * HoverAnimation * 0.55f), Col(11 + 132 * EasedSlide * 0.43f, 12 + 138 * EasedSlide * 0.43f, 18 + 237 * EasedSlide * 0.43f, 0), false);
	}
	Render::FilledCircle(x + Size.x - (62.5f - 25.f * EasedSlide * (OriginalMaxAlpha * 0.003922f)) * Menu->Scale, y + 8.5f * Menu->Scale, 10.f * Menu->Scale, Main, 17);

	


	return false;
}
bool Switch::ShouldRender() {

	if (ShouldRenderFn == nullptr)
		return true;

	bool ret = ShouldRenderFn();

	GUIAnimations::EaseAnimate(OffsetAnimation, ret, 0.0183f);


	return ret || OffsetAnimation > 0.f;
}
bool Switch::ShouldOverlay() {
	
	return Menu->Binder.Parent == this;

}
/*
float& Switch::GetAnimation() {
	return Slide;
}*/
void Switch::OnFree() {
	if (BindedVar == "") {
		throw IException("Menu Element has no Binded ConfigVar! (Memory Range Corrupt)", 0);
	}

	ConfigSystem->RemoveVar(BindedVar);
}


float Select::GetOffset() {
	return OffsetAnimation * Menu->Scale * 28.f;
}
bool Select::Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {
	float OriginalMaxAlpha = MaxAlpha;
	MaxAlpha *= GUIAnimations::Ease(OffsetAnimation);
	if (MaxAlpha != 255.f)
		Open = false;
	
	GUIAnimations::EaseAnimate(OpenAnimation, Open, 0.0178f);

	float W = 120 + 135 * HoverAnimation + 135 * OpenAnimation; 

	if (W > 255.f) {
		W = 255.f;
	}

	
	float ed = Render::TextSize(Fonts::MenuThin, (Elements[*Pointer].second).c_str()).x + 47.f * Menu->Scale;

	bool Hovered = Menu->InRegion(x + Size.x - ed - 18.f * Menu->Scale, y - 7.f * Menu->Scale, ed, 24.f * Menu->Scale) && !disable && OffsetAnimation == 1.f;

	if (Hovered && LeftClick && !Open) {
		LeftClick = false;
		Open = true;
		Scroll = 0.f;
		AnimatedScroll = 0.f;
	}
	else if (Hovered && Menu->MouseRightClick) {
		if (Menu->Binder.Parent == nullptr && Label != "Bind On Value" && Label != "Bind Off Value") {
			std::vector<std::string> El;
			for (auto& Element : Elements)
				El.push_back(Element.second);

			Menu->Binder.Update(this, BindedVar, El, 0, 0, BindtypeSelect);
			Menu->MouseRightClick = false;
			Menu->Binder.Open = true;
			Menu->Binder.Start = Menu->MousePos;
		}
	}


	


	GUIAnimations::Animate(HoverAnimation, Hovered);

	Render::DrawString(x + Size.x - ed, y, Col(W, W, W, MaxAlpha), Fonts::MenuThin, 0, (Elements[*Pointer].second).c_str());
	Render::DrawString(x + Size.x - 39.f * Menu->Scale, y - 1.f * Menu->Scale, Col(W, W, W, MaxAlpha * (OpenAnimation > 0.5f ? (OpenAnimation - 0.5f) * 2.f : (0.5f - OpenAnimation) * 2.f)), Fonts::MenuIcons, 0, OpenAnimation > 0.5f ? "Y" : "X");




	Render::DrawString(x, y, Col(W, W, W, MaxAlpha), Fonts::MenuThin, 0, Label.c_str());


	if (OpenAnimation > 0.f) {
		float Eased = GUIAnimations::Ease(OpenAnimation);

		x = x + Size.x - 90.f * Menu->Scale - 28.f * Menu->Scale;
		y = y + 25.f * Menu->Scale + (1.f - OpenAnimation) * 26.f * Menu->Scale;
		bool MainHovered = Menu->InRegion(x - 10.f * Menu->Scale, y, 130.f * Menu->Scale, 155.f * Menu->Scale);
		Render::FilledRoundedRect(x - 10.f * Menu->Scale, y, 130.f * Menu->Scale, 155.f * Menu->Scale, Col(0, 3, 6, MaxAlpha * Eased), 4.f * Menu->Scale);
		float Offset = 10.f * Menu->Scale;
		Render::PushClipRect(x - 10.f * Menu->Scale, y, 130.f * Menu->Scale, 155.f * Menu->Scale, true);
	
		y -= AnimatedScroll;
		int Count = 0;
		for (auto& Element : Elements) {
			bool Hovered = Menu->InRegion(x, y + Offset - Menu->Scale * 3.f, 130.f * Menu->Scale, (24.f + 2.f) * Menu->Scale) && Open && MainHovered;
			if (Hovered && LeftClick) {
				*Pointer = Count;
				LeftClick = false;
				Open = false;
				if (Config->AutoSave)
					ConfigSystem->SaveToConfig(ConfigSystem->Loaded);
			}
			GUIAnimations::Animate(Element.first, Hovered);
			W = 120 + 135 * Element.first;
			Render::DrawString(x, y + Offset, (Count == *Pointer) ? Col(170 + 30 * Element.first, 170 + 30 * Element.first, 255, MaxAlpha * Eased * (0.9f + 0.1f * Element.first)) : Col(W, W, W, MaxAlpha * Eased), Fonts::MenuThin, 0, Element.second.c_str());

			Offset += 24.f * Menu->Scale;
			Count++;
		}
	
		
		y += AnimatedScroll;
		bool ShouldScroll = Offset - 20.f * Menu->Scale > 155.f * Menu->Scale;
		if (ShouldScroll) {


			//	float MaxScroll = Math::Clamp(AnimatedScroll, 0.f, Offset);
			float OffsetY = (AnimatedScroll / (Offset - (24.f * Menu->Scale * 6.f + 10.f * Menu->Scale))) * (135.f * Menu->Scale);
			Render::FilledRoundedRectCustom(x + 115.f * Menu->Scale, y + OffsetY, 5.f * Menu->Scale, 20.f * Menu->Scale, Menu->MainTheme.WithAlpha(MaxAlpha * Eased), 4.f * Menu->Scale, ImDrawFlags_::ImDrawFlags_RoundCornersRight);
		}
		else
			Render::FilledRoundedRectCustom(x + 115.f * Menu->Scale, y, 5.f * Menu->Scale, 155.f * Menu->Scale, Col(11, 12, 18, MaxAlpha * Eased), 4.f * Menu->Scale, ImDrawFlags_::ImDrawFlags_RoundCornersRight);
		Render::PopClipRect();
		if (Open) {

			if (ShouldScroll) {
				bool Scrolled = false;
				while (Client->ScrollAmmount > 0) {
					Scroll -= 19.f * Menu->Scale;
					Client->ScrollAmmount--;
					Scrolled = true;
				}
				while (Client->ScrollAmmount < 0) {
					Scroll += 19.f * Menu->Scale;
					Client->ScrollAmmount++;
					Scrolled = true;
				}
				Offset -= 24.f * Menu->Scale * 6.f + 10.f * Menu->Scale;
				Math::ClampPtr(Scroll, Scrolled ? -100.f * Menu->Scale : 0.f, Scrolled ? (Offset + 100.f * Menu->Scale) : Offset);
			}
			else {
				bool Scrolled = false;

				while (Client->ScrollAmmount > 0) {
					Scroll -= 19.f * Menu->Scale;
					Client->ScrollAmmount--;
					Scrolled = true;
				}
				while (Client->ScrollAmmount < 0) {
					Scroll += 19.f * Menu->Scale;
					Client->ScrollAmmount++;
					Scrolled = true;
				}
				Math::ClampPtr(Scroll, -100.f, 100.f);

				if (!Scrolled)
					Scroll = 0.f;
			}


			AnimatedScroll = (AnimatedScroll + (Scroll - AnimatedScroll) * 0.02f * Menu->AnimationModifier);

			if (LeftClick && !MainHovered) {
				Open = false;
			}
		}
	}

	return false;
}
bool Select::ShouldRender() {

	if (ShouldRenderFn == nullptr)
		return true;

	bool ret = ShouldRenderFn();

	GUIAnimations::EaseAnimate(OffsetAnimation, ret, 0.0183f);


	return ret || OffsetAnimation > 0.f;
}
bool Select::ShouldOverlay() {
	return OpenAnimation > 0.f;
}
/*
float& Switch::GetAnimation() {
	return Slide;
}*/
void Select::OnFree() {
	if (BindedVar == "") {
		throw IException("Menu Element has no Binded ConfigVar! (Memory Range Corrupt)", 0);
	}

	ConfigSystem->RemoveVar(BindedVar);
}


float MultiSelect::GetOffset() {
	return OffsetAnimation * Menu->Scale * 28.f;
}
bool MultiSelect::Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {
	float OriginalMaxAlpha = MaxAlpha;
	MaxAlpha *= GUIAnimations::Ease(OffsetAnimation);
	if (MaxAlpha != 255.f)
		Open = false;

	GUIAnimations::EaseAnimate(OpenAnimation, Open, 0.0178f);

	float W = 120 + 135 * HoverAnimation + 135 * OpenAnimation;

	if (W > 255.f) {
		W = 255.f;
	}

	std::string Rendered = "";
	bool MoreThanOne = false;
	for (unsigned int Count = 0; Count < Elements.size(); Count++) {
		if ((*Pointer) & (1 << Count)) {
			if (Rendered == "")
				Rendered += Elements[Count].second;
			else
				MoreThanOne = true;
		}
	}
	if (Rendered == "")
		Rendered = "Select";
	else if(MoreThanOne)
		Rendered += "...";
	float ed = Render::TextSize(Fonts::MenuThin, (Rendered).c_str()).x + 47.f * Menu->Scale;

	bool Hovered = Menu->InRegion(x + Size.x - ed - 18.f * Menu->Scale, y - 7.f * Menu->Scale, ed, 24.f * Menu->Scale) && !disable && OffsetAnimation == 1.f;

	if (Hovered && LeftClick && !Open) {
		LeftClick = false;
		Open = true;
		Scroll = 0.f;
		AnimatedScroll = 0.f;
	}
	else if (Hovered && Menu->MouseRightClick) {
		if (Menu->Binder.Parent == nullptr && Label != "Bind On Value" && Label != "Bind Off Value") {
			std::vector<std::string> El;
			for (auto& Element : Elements)
				El.push_back(Element.second);

			Menu->Binder.Update(this, BindedVar, El, 0,0, BindtypeMultiselect);
			Menu->MouseRightClick = false;
			Menu->Binder.Open = true;
			Menu->Binder.Start = Menu->MousePos;
		}
	}




	GUIAnimations::Animate(HoverAnimation, Hovered);
	
	Render::DrawString(x + Size.x - ed, y, Col(W, W, W, MaxAlpha), Fonts::MenuThin, 0, (Rendered).c_str());
	Render::DrawString(x + Size.x - 39.f * Menu->Scale, y - 1.f * Menu->Scale, Col(W, W, W, MaxAlpha * (OpenAnimation > 0.5f ? (OpenAnimation - 0.5f) * 2.f : (0.5f - OpenAnimation) * 2.f)), Fonts::MenuIcons, 0, OpenAnimation > 0.5f ? "Y" : "X");





		Render::DrawString(x, y, Col(W, W, W, MaxAlpha), Fonts::MenuThin, 0, Label.c_str());


	if (OpenAnimation > 0.f) {
		float Eased = GUIAnimations::Ease(OpenAnimation);

		x = x + Size.x - 90.f * Menu->Scale - 28.f * Menu->Scale;
		y = y + 25.f * Menu->Scale + (1.f - OpenAnimation) * 26.f * Menu->Scale;
		bool MainHovered = Menu->InRegion(x - 10.f * Menu->Scale, y, 130.f * Menu->Scale, 155.f * Menu->Scale);
		Render::FilledRoundedRect(x - 10.f * Menu->Scale, y, 130.f * Menu->Scale, 155.f * Menu->Scale, Col(0, 3, 6, MaxAlpha * Eased), 4.f * Menu->Scale);
		float Offset = 10.f * Menu->Scale;
		Render::PushClipRect(x - 10.f * Menu->Scale, y, 130.f * Menu->Scale, 155.f * Menu->Scale, true);

		y -= AnimatedScroll;
		unsigned int Count = 0;
		for (auto& Element : Elements) {
			bool Hovered = Menu->InRegion(x, y + Offset - Menu->Scale * 3.f, 130.f * Menu->Scale, (24.f + 2.f) * Menu->Scale) && Open && MainHovered;
			if (Hovered && LeftClick) {

				if ((*Pointer) & (1 << Count)) {
					*Pointer &= ~(1 << Count);
				}
				else
					*Pointer |= (1 << Count);

				LeftClick = false;
				if (Config->AutoSave)
					ConfigSystem->SaveToConfig(ConfigSystem->Loaded);
			}
			GUIAnimations::Animate(Element.first, Hovered);
			W = 120 + 135 * Element.first;
			Render::DrawString(x, y + Offset, (*Pointer) & (1 << Count) ? Col(170 + 30 * Element.first, 170 + 30 * Element.first, 255, MaxAlpha * Eased * (0.9f + 0.1f * Element.first)) : Col(W, W, W, MaxAlpha * Eased), Fonts::MenuThin, 0, Element.second.c_str());

			Offset += 24.f * Menu->Scale;
			Count++;
		}


		y += AnimatedScroll;
		bool ShouldScroll = Offset - 20.f * Menu->Scale > 155.f * Menu->Scale;
		if (ShouldScroll) {


			//	float MaxScroll = Math::Clamp(AnimatedScroll, 0.f, Offset);
			float OffsetY = (AnimatedScroll / (Offset - (24.f * Menu->Scale * 6.f + 10.f * Menu->Scale))) * (135.f * Menu->Scale);
			Render::FilledRoundedRectCustom(x + 115.f * Menu->Scale, y + OffsetY, 5.f * Menu->Scale, 20.f * Menu->Scale, Menu->MainTheme.WithAlpha(MaxAlpha * Eased), 4.f * Menu->Scale, ImDrawFlags_::ImDrawFlags_RoundCornersRight);
		}
		else
			Render::FilledRoundedRectCustom(x + 115.f * Menu->Scale, y, 5.f * Menu->Scale, 155.f * Menu->Scale, Col(11, 12, 18, MaxAlpha * Eased), 4.f * Menu->Scale, ImDrawFlags_::ImDrawFlags_RoundCornersRight);
		Render::PopClipRect();
		if (Open) {

			if (ShouldScroll) {
				bool Scrolled = false;
				while (Client->ScrollAmmount > 0) {
					Scroll -= 19.f * Menu->Scale;
					Client->ScrollAmmount--;
					Scrolled = true;
				}
				while (Client->ScrollAmmount < 0) {
					Scroll += 19.f * Menu->Scale;
					Client->ScrollAmmount++;
					Scrolled = true;
				}
				Offset -= 24.f * Menu->Scale * 6.f + 10.f * Menu->Scale;
				Math::ClampPtr(Scroll, Scrolled ? -100.f * Menu->Scale : 0.f, Scrolled ? (Offset + 100.f * Menu->Scale) : Offset);
			}
			else {
				bool Scrolled = false;

				while (Client->ScrollAmmount > 0) {
					Scroll -= 19.f * Menu->Scale;
					Client->ScrollAmmount--;
					Scrolled = true;
				}
				while (Client->ScrollAmmount < 0) {
					Scroll += 19.f * Menu->Scale;
					Client->ScrollAmmount++;
					Scrolled = true;
				}
				Math::ClampPtr(Scroll, -100.f, 100.f);

				if (!Scrolled)
					Scroll = 0.f;
			}


			AnimatedScroll = (AnimatedScroll + (Scroll - AnimatedScroll) * 0.02f * Menu->AnimationModifier);

			if (LeftClick && !MainHovered) {
				Open = false;
			}
		}
	}

	return false;
}
bool MultiSelect::ShouldRender() {

	if (ShouldRenderFn == nullptr)
		return true;

	bool ret = ShouldRenderFn();

	GUIAnimations::EaseAnimate(OffsetAnimation, ret, 0.0183f);


	return ret || OffsetAnimation > 0.f;
}
bool MultiSelect::ShouldOverlay() {
	return OpenAnimation > 0.f;
}
/*
float& Switch::GetAnimation() {
	return Slide;
}*/
void MultiSelect::OnFree() {
	if (BindedVar == "") {
		throw IException("Menu Element has no Binded ConfigVar! (Memory Range Corrupt)", 0);
	}

	ConfigSystem->RemoveVar(BindedVar);
}
float Slider::GetOffset() {
	return OffsetAnimation * Menu->Scale * 28.f;
}
bool Slider::Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {
	float OriginalMaxAlpha = MaxAlpha;
	MaxAlpha *= GUIAnimations::Ease( OffsetAnimation );

	if (MaxAlpha != 255.f)
		TextOpen = false;
//	bool Hovered = Menu->InRegion(x + Size.x - 76.f * Menu->Scale, y - 6.f * Menu->Scale, 47.f * Menu->Scale, 30.f * Menu->Scale) && !disable && OffsetAnimation == 1.f;

//	if ((Hovered && HoverAnimation < 1.f) || (!Hovered && HoverAnimation > 0.f))
//		HoverAnimation = Math::Clamp(HoverAnimation + ((Hovered ? 1.f : -1.f) * 0.006f * Menu->AnimationModifier), 0.f, 1.f);
//
	//bool HoveredOverText = Menu->InRegion()
	if (Config->AutoSave) {
		if (OldPointer != *Pointer) {
			*Pointer = Math::Clamp(*Pointer, MinValue, MaxValue);
			OldPointer = *Pointer;
			ConfigSystem->SaveToConfig(ConfigSystem->Loaded);
		}
	}
	/*
	if (Render::TextSize(Fonts::MenuThin, Label.c_str()).x > Size.x - 75.f * Menu->Scale - Size.x * 0.35f - 45.f * Menu->Scale) {
		Render::PushClipRect(x, y - 4.f * Menu->Scale, Size.x - 75.f * Menu->Scale - Size.x * 0.35f - 5.f * Menu->Scale, 23.f * Menu->Scale, true);
		Render::DrawString(x, y, Col(255,255,255, MaxAlpha), Fonts::MenuThin, 0, Label.c_str());
		Col Full = Col(0, 1, 2, OriginalMaxAlpha);
		Col Low = Col(0, 1, 2, 0);
		//note: rendered twice for quadratic effect
		Render::GradientFilledRect(x + Size.x - 75.f * Menu->Scale - Size.x * 0.35f - 28.f * Menu->Scale, y - 4.f * Menu->Scale, 23.f * Menu->Scale, 26.f * Menu->Scale, Low, Full, Low, Full);
		Render::GradientFilledRect(x + Size.x - 75.f * Menu->Scale - Size.x * 0.35f - 28.f * Menu->Scale, y - 4.f * Menu->Scale, 23.f * Menu->Scale, 26.f * Menu->Scale, Low, Full, Low, Full);
		Render::GradientFilledRect(x + Size.x - 75.f * Menu->Scale - Size.x * 0.35f - 28.f * Menu->Scale, y - 4.f * Menu->Scale, 23.f * Menu->Scale, 26.f * Menu->Scale, Low, Full, Low, Full);
		Render::PopClipRect();
	}
	else*/
		Render::DrawString(x, y, Col(255, 255, 255, MaxAlpha), Fonts::MenuThin, 0, Label.c_str());

	Col Main(11 + 132, 12 + 138 , 18 + 237, MaxAlpha);
	Col Background(0, 3, 6, MaxAlpha * 0.78f);





	//ok time for annoying shit the math of the circle thing
	float ToModifierX =  (abs((*Pointer) - MinValue) * MinMaxDivisor) * MaxAlpha * 0.003922f;
	ModifierX = (ModifierX + (ToModifierX - ModifierX) * 0.02f * Menu->AnimationModifier);
	Math::ClampPtr(ModifierX, 0.f, 1.f);
	float ModifierX2 = Size.x * 0.35f * ModifierX;
	bool bHovered = Menu->InRegion(x + Size.x - 83.f * Menu->Scale - Size.x * 0.35f, y - 10.f * Menu->Scale, Size.x * 0.35f + 32.f * Menu->Scale, 23.f * Menu->Scale) && !disable && OffsetAnimation == 1.f;
	if (Drag && bHovered) {
		float newpos = Menu->MousePos.x - (x + Size.x - 75.f * Menu->Scale - Size.x * 0.35f);
		if (newpos < 0)
			newpos = 0;
		if (newpos > Size.x * 0.35f)
			newpos = Size.x * 0.35f;
		float ratio = newpos / (Size.x * 0.35f);

		*Pointer = (int)(MinValue + (MaxValue - MinValue) * ratio);

		Drag = false;
	}
	else if (bHovered && Menu->MouseRightClick) {
		if (Menu->Binder.Parent == nullptr && Label != "Bind On Value" && Label != "Bind Off Value") {
			Menu->Binder.Update(this, BindedVar, {}, MinValue, MaxValue, BindtypeSlider);
			Menu->MouseRightClick = false;
			Menu->Binder.Open = true;
			Menu->Binder.Start = Menu->MousePos;
		}
	}

	GUIAnimations::Animate(HoverAnimation, bHovered);


	Render::FilledRoundedRect(x + Size.x - 75.f * Menu->Scale - Size.x * 0.35f, y + 4.f * Menu->Scale, Size.x * 0.35f, 8.f * Menu->Scale, Background, 30.f * Menu->Scale);
	if (HoverAnimation > 0) {
		Render::GradientCircle(x + Size.x - 75.f * Menu->Scale - Size.x * 0.35f + ModifierX2, y + 8.5f * Menu->Scale, 14.f * Menu->Scale, Col(11 + 132 *0.43f, 12 + 138 *0.43f, 18 + 237 *0.43f, MaxAlpha * HoverAnimation * 0.55f), Col(11 + 132 * 0.43f, 12 + 138 * 0.43f, 18 + 237 * 0.43f, 0), false);
	}
	Render::FilledCircle(x + Size.x - 75.f * Menu->Scale - Size.x * 0.35f + ModifierX2, y + 8.f * Menu->Scale, 7.f * Menu->Scale, Main, 17);


	bool HoveredOverText = Menu->InRegion(x + Size.x - 56.f * Menu->Scale, y - 2.f * Menu->Scale, 38.f * Menu->Scale, 24.f * Menu->Scale) && !disable && OffsetAnimation == 1.f;
	GUIAnimations::Animate(HoverTextAnimation, HoveredOverText || TextOpen);
	if (HoveredOverText) {
		if (LeftClick) {
			LeftClick = false;
			TextOpen = true;
			Menu->EditTextAnimation = Menu->CurrentClock;
		}
		else if (Menu->MouseRightClick) {
			if (Menu->Binder.Parent == nullptr && Label != "Bind On Value" && Label != "Bind Off Value") {
				Menu->Binder.Update(this, BindedVar, {}, MinValue, MaxValue, BindtypeSlider);
				Menu->MouseRightClick = false;
				Menu->Binder.Open = true;
				Menu->Binder.Start = Menu->MousePos;
			}
		}
		ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
	}
	else if (LeftClick && TextOpen) {
		TextOpen = false;
		if (EditableText != "") {

			*Pointer = std::stoi(EditableText);
			TextOpen = false;
		}
		else {
			TextOpen = false;
			EditableText = std::to_string(*Pointer);
		}
		if (LeftClick)
			LeftClick = false;
	}

	std::string Written = EditableText;
	if (TextOpen) {
		if ((int((Menu->CurrentClock - Menu->EditTextAnimation) * 0.0019f) % 2) == 0)
			Written += "|";
		else
			Written += " ";

		ImGui::GetIO().WantTextInput = true;

		if (ImGui::GetIO().InputQueueCharacters.Size > 0) {
			for (auto c : ImGui::GetIO().InputQueueCharacters) {

				if (c == VK_RETURN) {

					if (EditableText != "") {

						*Pointer = std::stoi(EditableText);
						TextOpen = false;
					}
					else {
						TextOpen = false;
						EditableText = std::to_string(*Pointer);
					}
					if (LeftClick)
						LeftClick = false;
					continue;
				}

				if (c == VK_ESCAPE) {
					TextOpen = false;
					EditableText = std::to_string(*Pointer);
					if (LeftClick)
						LeftClick = false;
					continue;
				}
				else if (c == VK_BACK)
					EditableText = EditableText.substr(0, EditableText.size() - 1);
				else if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9') {
					if (EditableText.size() < 40)
						EditableText += (unsigned char)c;
				}
			}
		}
	}
	else
		EditableText = std::to_string(*Pointer);


	Render::FilledRoundedRect(x + Size.x - 59.f * Menu->Scale, y - 3.f * Menu->Scale, 32.f * Menu->Scale, 22.f * Menu->Scale, Col(0, 0, 1, MaxAlpha * (0.36f + 0.5f * HoverTextAnimation)), 3.f * Menu->Scale);
	Render::RoundedRect(x + Size.x - 59.f * Menu->Scale, y - 3.f * Menu->Scale, 32.f * Menu->Scale, 22.f * Menu->Scale, Col(100 + 155 * HoverTextAnimation, 104 + 151 * HoverTextAnimation, 110 + 145 * HoverTextAnimation, MaxAlpha * (0.36f + 0.5f * HoverTextAnimation)), 0.9f * Menu->Scale, 3.f * Menu->Scale);
	Render::PushClipRect(x + Size.x - 59.f * Menu->Scale, y - 3.f * Menu->Scale, 32.f * Menu->Scale, 22.f * Menu->Scale, true);
	Render::DrawString(x + Size.x - 42.5f * Menu->Scale, y - 1.5f * Menu->Scale, Col(255, 255, 255, MaxAlpha), Fonts::MenuThin, Render::centered_x, Written.c_str());
	Render::PopClipRect();
	return false;
}
bool Slider::ShouldRender() {
	if (ShouldRenderFn == nullptr)
		return true;

	bool ret = ShouldRenderFn();

	GUIAnimations::EaseAnimate(OffsetAnimation, ret, 0.0183f);


	return ret || OffsetAnimation > 0.f;
}
bool Slider::ShouldOverlay() {
	return false;
}

void Slider::OnFree() {
	if (BindedVar == "") {
		throw IException("Menu Element has no Binded ConfigVar! (Memory Range Corrupt)", 0);
	}

	ConfigSystem->RemoveVar(BindedVar);
}

float ConfigView::GetOffset() {
	return 0.f;
}
#include <json.h>
void ConfigView::DrawConfig(float x, float y, float MaxAlpha, std::string label, ConfigView::ConfigAnimation& Context, bool& LeftClick, bool ShouldDoUpdate) {

	MaxAlpha *= Context.Offset;

	bool MenuHovered = Menu->InRegion(x, y, 193.f * Menu->Scale, 150.f * Menu->Scale) && !Context.ToBeDeleted && ShouldDoUpdate;
	bool DeleteHovered = Menu->InRegion(x + 30.f * Menu->Scale, y + 125.f * Menu->Scale, 34.f * Menu->Scale, 44.f * Menu->Scale) && !Context.ToBeDeleted && ShouldDoUpdate && label != "Default";
	bool CopyHovered = Menu->InRegion(x, y + 125.f * Menu->Scale, 30.f * Menu->Scale, 44.f * Menu->Scale) && !Context.ToBeDeleted && ShouldDoUpdate;

	GUIAnimations::Animate(Context.Copy, CopyHovered);

	if((MenuHovered && Context.Text < 1.f) || (!MenuHovered && Context.Text > 0.f))
		Context.Text = Math::Clamp(Context.Text + ((MenuHovered ? 1.f : -1.f) * 0.007f * Menu->AnimationModifier), 0.f, 1.f);

	if (label == ConfigSystem->Loaded)
		Context.Text = 1.f;

	if ((DeleteHovered && Context.Delete < 1.f) || (!DeleteHovered && Context.Delete > 0.f))
		Context.Delete = Math::Clamp(Context.Delete + ((DeleteHovered ? 1.f : -1.f) * 0.007f * Menu->AnimationModifier), 0.f, 1.f);


	if ((!Context.ToBeDeleted && Context.Offset < 1.f) || (Context.ToBeDeleted && Context.Offset > 0.f))
		Context.Offset = Math::Clamp(Context.Offset + ((Context.ToBeDeleted ? -1.f : 1.f) * 0.007f * Menu->AnimationModifier), 0.f, 1.f);

	if (Context.ToBeDeleted && Context.Offset <= 0.f) {
		ConfigSystem->RemoveConfig(label);
		Reload();
		ToErase.push_back(label);
		
		return;
	}

	if (LeftClick && DeleteHovered) {
		Context.ToBeDeleted = true;
		LeftClick = false;
	}

	if (LeftClick && CopyHovered) {
		LeftClick = false;
		Reload();
		
		if (ConfigSystem->Configs.find(label) != ConfigSystem->Configs.end()) {

			std::string File = Client->InfFolder + "\\" + label + ".infinite";



			std::ifstream FileReader(File);

			if (!FileReader.good()) {
				FileReader.close();
				return;
			}


			Json::Value Root;
			FileReader >> Root;
			const std::string Original = label;
			std::string NewConfigName = Original + " Copy";
			

			std::string File2 = Client->InfFolder + "\\" + NewConfigName + ".infinite";
			std::ofstream FileWriter(File2);

			if (!FileWriter.good()) {
				FileWriter.close();
				return;
			}

			FileWriter << Root;
			FileWriter.close();
			Root.clear();

			Reload();
		}
	}

	if (LeftClick && MenuHovered && !DeleteHovered) {
		Reload();
		if (ConfigViews.find(label) != ConfigViews.end()) {
			SelectedConfig = label;
			EditableText = label;
			LastSelectedConfig = label;
			Scroll = 0.f;

			SelectionOpenAnimation = 0.f;
			SelectionBackAnimation = 0.f;
			SelectionSaveAnimation = 0.f;
			SelectionLoadAnimation = 0.f;
			SelectionCopyAnimation = 0.f;
			SelectionRenameAnimation = 0.f;
			SelectionAuthorInfoAnimation = 0.f;
			SelectionInRename = false;
		}
		LeftClick = false;
	}


	Render::FilledRoundedRect(x, y, 193.f * Menu->Scale, 150.f * Menu->Scale, Col(0 , 3 , 6 , MaxAlpha * (0.55f + Context.Text * 0.45f)), 5.f * Menu->Scale);

	if (Render::TextSize(Fonts::MenuThin, label.c_str()).x > 193.f * Menu->Scale - 22.f * Menu->Scale) {
		Render::PushClipRect(x + 10.f * Menu->Scale, y, 173.f * Menu->Scale, 150.f * Menu->Scale, true);
		Render::DrawString(x + 96.5f * Menu->Scale, y + 75.f * Menu->Scale, Col(255,255,255, MaxAlpha), Fonts::MenuThin, Render::centered_xy, label.c_str());
		Render::PopClipRect();
	}
	else
		Render::DrawString(x + 96.5f * Menu->Scale, y + 75.f * Menu->Scale, Col(255,255,255, MaxAlpha), Fonts::MenuThin, Render::centered_xy, label.c_str());
	
	if (label != "Default") {
		Render::DrawString(x + 50.f * Menu->Scale, y + 130.f * Menu->Scale, Col(55 + Context.Delete * 200, 55 + Context.Delete * 200, 55 + Context.Delete * 200, MaxAlpha * (0.5f + 0.5f * Context.Delete)), Fonts::MenuIcons, Render::centered_xy, "T");
		Render::DrawString(x + 20.f * Menu->Scale, y + 130.f * Menu->Scale, Col(55 + Context.Copy * 200, 55 + Context.Copy * 200, 55 + Context.Copy * 200, MaxAlpha * (0.5f + 0.5f * Context.Copy)), Fonts::MenuIcons, Render::centered_xy, CopyHovered ? "B" : "A");

	}
	else
		Render::DrawString(x + 20.f * Menu->Scale, y + 130.f * Menu->Scale, Col(55 + Context.Copy * 200, 55 + Context.Copy * 200, 55 + Context.Copy * 200, MaxAlpha * (0.5f + 0.5f * Context.Copy)), Fonts::MenuIcons, Render::centered_xy, CopyHovered ? "B" : "A");
}

bool ConfigView::Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {

	if (MaxAlpha != 255.f && (int(Menu->CurrentClock * 0.08f) % 2) == 0) {
		if (ConfigSystem->Configs.empty()) {
			ConfigSystem->CreateConfig("Default");
			ConfigSystem->LoadToConfig("Default");

			ConfigSystem->Loaded = "Default";
		}
		else if (ConfigSystem->Configs.find("Default") == ConfigSystem->Configs.end()) {
			ConfigSystem->CreateConfig("Default");
		}

		while (ConfigSystem->Configs.find("Default") == ConfigSystem->Configs.end())
			Reload();


		Reload();
	}

	Vec2 OrP	(x, y);
	Render::PushClipRect(x, y, Size.x - 40.f * Menu->Scale, Size.y, true);
	x += 17.f * Menu->Scale;
	y += 40.f * Menu->Scale;

	if ((SelectedConfig != "" && SelectionOpenAnimation < 1.f) || (SelectedConfig == "" && SelectionOpenAnimation > 0.f))
		SelectionOpenAnimation = Math::Clamp(SelectionOpenAnimation + ((SelectedConfig != "" ? 1.f : -1.f) * 0.007f * Menu->AnimationModifier), 0.f, 1.f);




	if (ConfigViews.size() >= 6) {
		bool Scrolled = false;
		while (Client->ScrollAmmount > 0) {
			Scroll -= 30.f * Menu->Scale;
			Client->ScrollAmmount--;
			Scrolled = true;
		}
		while (Client->ScrollAmmount < 0) {
			Scroll += 30.f * Menu->Scale;
			Client->ScrollAmmount++;
			Scrolled = true;
		}
		Math::ClampPtr(Scroll, Scrolled ? -100.f * Menu->Scale : 0.f, Scrolled ? (180.f * Menu->Scale * (ConfigViews.size() - 3) * 0.33333333f + 100.f * Menu->Scale) : (180.f * Menu->Scale * (ConfigViews.size() - 3) * 0.33333333f));
	}
	else {
		bool Scrolled = false;

		while (Client->ScrollAmmount > 0) {
			Scroll -= 30.f * Menu->Scale;
			Client->ScrollAmmount--;
			Scrolled = true;
		}
		while (Client->ScrollAmmount < 0) {
			Scroll += 30.f * Menu->Scale ;
			Client->ScrollAmmount++;
			Scrolled = true;
		}
		Math::ClampPtr(Scroll,-100.f, 100.f);

		if (!Scrolled)
			Scroll = 0.f;
	}


	AnimatedScroll = (AnimatedScroll + (Scroll - AnimatedScroll) * 0.02f * Menu->AnimationModifier);

	y -= AnimatedScroll;
	int Count = 0;
	float AddX = 0.f;

	for (auto& Context : ConfigViews) {

		if (Count == 3) {
			Count = 0;

			y += 180.f * Menu->Scale;
				
			AddX = 0.f;
			
		}
		
		DrawConfig(x + AddX, y, MaxAlpha * (1.f - SelectionOpenAnimation), Context.first, Context.second, LeftClick, SelectionOpenAnimation == 0.f);

		AddX += 223.f * Menu->Scale;


		Count++;
	
	}
	if (Count == 3) {
		Count = 0;

		y += 180.f * Menu->Scale;

		AddX = 0.f;

	}
	bool NewConfig = Menu->InRegion(x + AddX, y, 193.f * Menu->Scale, 150.f * Menu->Scale) && SelectionOpenAnimation == 0.f;
	GUIAnimations::Animate(SelectionCopyAnimation, NewConfig);
	Render::FilledRoundedRect(x + AddX, y, 193.f * Menu->Scale, 150.f * Menu->Scale, Col(0, 3, 6, (MaxAlpha * (1.f - SelectionOpenAnimation)) * (0.55f + SelectionCopyAnimation * 0.45f)), 5.f * Menu->Scale);
	Render::DrawString(x + 96.5f * Menu->Scale + AddX, y + 75.f * Menu->Scale, Col(255, 255, 255, (MaxAlpha * (1.f - SelectionOpenAnimation)) * 0.4f), Fonts::MenuIcons, Render::centered_xy, "P");

	if (NewConfig && LeftClick) {
		LeftClick = false;
		std::string File = Client->InfFolder + "\\" + ConfigSystem->Loaded + ".infinite";



		std::ifstream FileReader(File);

		if (!FileReader.good()) {
			FileReader.close();
			return false;
		}


		Json::Value Root;
		FileReader >> Root;
	
		int It = 2;
		std::string NewConfigName = "New Config";
		while (ConfigSystem->Configs.find(NewConfigName) != ConfigSystem->Configs.end()) {
			NewConfigName = "New Config " + std::to_string(It);
			It++;
		}


		std::string File2 = Client->InfFolder + "\\" + NewConfigName + ".infinite";
		std::ofstream FileWriter(File2);

		if (!FileWriter.good()) {
			FileWriter.close();
			return false;
		}

		FileWriter << Root;
		FileWriter.close();
		Root.clear();

		Reload();
		/*
		int It = 2;
		std::string NewConfigName = "New Config ";
		while (ConfigSystem->Configs.find(NewConfigName) != ConfigSystem->Configs.end()) {
			NewConfigName = "New Config " + std::to_string(It);
			It++;
		}
		ConfigSystem->CreateConfig(NewConfigName);
		//create config

			Reload();
		*/
	}

	Col Full = Col(0, 1, 2, MaxAlpha);
	Col Low = Col(0, 1, 2, 0);
	x = OrP.x;
	y = OrP.y;
	Render::PopClipRect();
	Render::GradientFilledRect(x, y + Size.y - 50.f * Menu->Scale, Size.x - 20.f * Menu->Scale, 80.f * Menu->Scale, Low, Low, Full, Full);
	Render::GradientFilledRect(x, y + Size.y - 50.f * Menu->Scale, Size.x - 20.f * Menu->Scale, 80.f * Menu->Scale, Low, Low, Full, Full);
	Render::GradientFilledRect(x, y + Size.y - 50.f * Menu->Scale, Size.x - 20.f * Menu->Scale, 80.f * Menu->Scale, Low, Low, Full, Full);

	if (SelectedConfig != "" || SelectionOpenAnimation > 0.f) {
		MaxAlpha *= SelectionOpenAnimation;
		if (MaxAlpha != 255.f)
		{
			SelectionInRename = false;
			EditableText = LastSelectedConfig;
		}
		Render::FilledRoundedRect(x - 15.f * Menu->Scale, y + 13.f * Menu->Scale, Size.x, Size.y, Col(0, 1, 2, MaxAlpha), 4.5f * Menu->Scale);
		Render::DrawString(x + 25.f * Menu->Scale, y + 48.f * Menu->Scale, Col(130 + 125 * SelectionBackAnimation, 130 + 125 * SelectionBackAnimation, 130 + 125 * SelectionBackAnimation, MaxAlpha), Fonts::MenuIcons, Render::centered_xy, "M");

		Render::DrawString(x + 60.f * Menu->Scale, y + 90.f * Menu->Scale, Col(255, 255, 255, MaxAlpha), Fonts::MenuThin, 0, "Config: ");

		bool HoveredOverCfg = Menu->InRegion(x + 127.f * Menu->Scale, y + 84.f * Menu->Scale, 470.f * Menu->Scale, 29.f * Menu->Scale) && LastSelectedConfig != "Default";

		GUIAnimations::Animate(SelectionRenameAnimation, HoveredOverCfg || SelectionInRename);
		std::string WrittenCFG = EditableText;
		if (HoveredOverCfg) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
			if (LeftClick) {
				SelectionInRename = true;
				Menu->EditTextAnimation = Menu->CurrentClock;
			}

		}
		else if (LeftClick && SelectionInRename) {
			SelectionInRename = false;
			if (EditableText == "") {
				EditableText = LastSelectedConfig;
			}
			else {
				ConfigSystem->RebindConfig(LastSelectedConfig, EditableText);
				std::string OldSelected = LastSelectedConfig;
				LastSelectedConfig = EditableText;
				SelectedConfig = EditableText;
				Reload();
				if (ConfigSystem->Loaded == OldSelected) {
					ConfigSystem->Loaded = SelectedConfig;
					ConfigSystem->LoadToConfig(ConfigSystem->Loaded);
				}
			}
			
		}

		if (SelectionInRename) {
			ImGui::GetIO().WantTextInput = true;
			if ((int((Menu->CurrentClock - Menu->EditTextAnimation) * 0.0019f) % 2) == 0)
				WrittenCFG += "|";

			if (ImGui::GetIO().InputQueueCharacters.Size > 0) {
				for (auto c : ImGui::GetIO().InputQueueCharacters) {

					if (c == VK_RETURN) {
						SelectionInRename = false;
						if (EditableText == "") {
							EditableText = LastSelectedConfig;
						}
						else {
							ConfigSystem->RebindConfig(LastSelectedConfig, EditableText);
							std::string OldSelected = LastSelectedConfig;
							LastSelectedConfig = EditableText;
							SelectedConfig = EditableText;
							Reload();
							if (ConfigSystem->Loaded == OldSelected) {
								ConfigSystem->Loaded = SelectedConfig;
								ConfigSystem->LoadToConfig(ConfigSystem->Loaded);
							}
						}

						if (LeftClick)
							LeftClick = false;
						continue;
					}

					if (c == VK_ESCAPE) {
						SelectionInRename = false;
						EditableText = LastSelectedConfig;
						if (LeftClick)
							LeftClick = false;
						continue;
					}
					else if (c == VK_BACK)
						EditableText = EditableText.substr(0, EditableText.size() - 1);
					else if (c != VK_TAB) {
						if (EditableText.size() < 40)
							EditableText += (unsigned char)c;
					}
				}
			}
		}
		if (LastSelectedConfig != "Default") {
			Render::FilledRoundedRect(x + 127.f * Menu->Scale, y + 84.f * Menu->Scale, 470.f * Menu->Scale, 29.f * Menu->Scale, Col(0, 0, 1, MaxAlpha * (0.16f + 0.1f * SelectionRenameAnimation)), 3.f * Menu->Scale);
			Render::RoundedRect(x + 127.f * Menu->Scale, y + 84.f * Menu->Scale, 470.f * Menu->Scale, 29.f * Menu->Scale, Col(100 + 60 * SelectionRenameAnimation, 104 + 60 * SelectionRenameAnimation, 110 + 60 * SelectionRenameAnimation, MaxAlpha * 0.21f), 0.9f * Menu->Scale, 3.f * Menu->Scale);
		}
		Render::DrawString(x + 135.f * Menu->Scale, y + 90.f * Menu->Scale, Col(150, 164, 255, MaxAlpha), Fonts::MenuThin, 0, WrittenCFG.c_str());


		Render::DrawString(x + 60.f * Menu->Scale, y + 130.f * Menu->Scale, Col(255, 255, 255, MaxAlpha), Fonts::MenuThin, 0, "Author: ");

		bool HoveredOverAuthor = Menu->InRegion(x + 125.f * Menu->Scale, y + 125.f * Menu->Scale, Render::TextSize(Fonts::MenuThin, "test").x + 20.f * Menu->Scale, 30.f);
		GUIAnimations::Animate(SelectionAuthorInfoAnimation, HoveredOverAuthor);
		Render::DrawString(x + 135.f * Menu->Scale, y + 130.f * Menu->Scale, Col(150 + 50 * SelectionAuthorInfoAnimation, 164 + 50 * SelectionAuthorInfoAnimation, 255, MaxAlpha), Fonts::MenuThin, 0, "test");
		Render::FilledRect(x + 135.f * Menu->Scale, y + 146.f * Menu->Scale, Render::TextSize(Fonts::MenuThin, "test").x, 1.2f * Menu->Scale, Col(150, 164, 255, MaxAlpha));
		Render::DrawString(x + 60.f * Menu->Scale, y + 170.f * Menu->Scale, Col(255, 255, 255, MaxAlpha), Fonts::MenuThin, 0, "Last Modified: ");
		Render::DrawString(x + 183.f * Menu->Scale, y + 170.f * Menu->Scale, Col(150, 164, 255, MaxAlpha), Fonts::MenuThin, 0, ConfigViews[LastSelectedConfig].Config->Data.LastModified.c_str());
		bool HoveringBack = Menu->InRegion(x + 10.f * Menu->Scale, y + 33.f * Menu->Scale, 35.f, 35.f);
		GUIAnimations::Animate(SelectionBackAnimation, HoveringBack);

		//SelectionLoadAnimation -> HOVER
		Vec2 TextSize1 = Render::TextSize(Fonts::MenuMain, LastSelectedConfig == ConfigSystem->Loaded ? "Save" : "Load");
		bool HoveringSave = !SelectionInRename && Menu->InRegion(x + 530.f * Menu->Scale - TextSize1.x * 0.5f, y + 367.f * Menu->Scale - TextSize1.y * 0.5f, TextSize1.x + 19.f * Menu->Scale, TextSize1.y + 10.f * Menu->Scale);
		
		if (HoveringSave && LeftClick) {
			if (LastSelectedConfig == ConfigSystem->Loaded) {
				ConfigViews[LastSelectedConfig].Config->Save();
			}
			else {
				ConfigViews[LastSelectedConfig].Config->Load();
				ConfigSystem->Loaded = LastSelectedConfig;
			}
		}
		
		GUIAnimations::Animate(SelectionLoadAnimation, HoveringSave);
		GUIAnimations::Animate(SelectionSaveAnimation, LastSelectedConfig != ConfigSystem->Loaded);
		if (ConfigSystem->Loaded != LastSelectedConfig) {
			Render::FilledRoundedRect(x + 529.f * Menu->Scale - TextSize1.x * 0.5f, y + 367.f * Menu->Scale - TextSize1.y * 0.5f, TextSize1.x + 22.f * Menu->Scale, TextSize1.y + 10.f * Menu->Scale, Col((108 + SelectionLoadAnimation * 60) * SelectionSaveAnimation, (108 + SelectionLoadAnimation * 60) * SelectionSaveAnimation, (232 + SelectionLoadAnimation * 23) * SelectionSaveAnimation, MaxAlpha), 6.f * Menu->Scale);
		}
		else {
			Render::FilledRoundedRect(x + 529.f * Menu->Scale - TextSize1.x * 0.5f, y + 367.f * Menu->Scale - TextSize1.y * 0.5f, TextSize1.x + 22.f * Menu->Scale, TextSize1.y + 10.f * Menu->Scale, Col(80, 90, 170, MaxAlpha * (0.01f + SelectionLoadAnimation * 0.24f)), 6.f * Menu->Scale);
		}
		
		Render::RoundedRect (x + 529.f * Menu->Scale - TextSize1.x * 0.5f, y + 367.f * Menu->Scale - TextSize1.y * 0.5f, TextSize1.x + 22.f * Menu->Scale, TextSize1.y + 10.f * Menu->Scale, Col(100,104,110, MaxAlpha), 1.f * Menu->Scale, 6.f * Menu->Scale);
		Render::DrawString(x + 540.f * Menu->Scale, y + 370.f * Menu->Scale, Col(255, 255, 255, MaxAlpha), Fonts::MenuMain, Render::centered_xy, LastSelectedConfig == ConfigSystem->Loaded ?  "Save" : "Load");
		if (SelectedConfig != "") {
			if (HoveringBack && LeftClick) {
				LastSelectedConfig = SelectedConfig;
				SelectedConfig = "";
			}

		}

	}

	for (auto& Erasable : ToErase) {
		ConfigViews.erase(Erasable);
	}
	ToErase.clear();

	return false;
}
bool ConfigView::ShouldRender() {
	return true;
}
bool ConfigView::ShouldOverlay() {
	return false;
}
void ConfigView::OnFree() {
	throw IException("Tried to free ConfigView! (??)", 0);
}