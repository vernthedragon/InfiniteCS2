#include "GUI.h"
#include "Menu.h"

float GUIAnimations::Ease(const float& a) {
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

#pragma region GUIClass
void Child::Draw(float x, float y, float MaxAlpha, bool LeftClick, bool Drag) {

	if ((Open && OpenAnimation < 1.f) || (!Open && OpenAnimation > 0.f))
		OpenAnimation = Math::Clamp(OpenAnimation + ((Open ? 1.f : -1.f) * 0.0053f * Menu->AnimationModifier), 0.f, 1.f);

	MaxAlpha *= GUIAnimations::Ease(OpenAnimation);
	Background[3] = MaxAlpha;
		//NOTE THIS->SIZE IS SCALED IN MENU.CPP
	Render::FilledRoundedRect(x, y, Size.x, Size.y, Background, 4.5f * Menu->Scale);


	
	Render::PushClipRect(x,y, Size.x, Size.y, true);
	
	
	MenuElement* Overlay = nullptr;
	float OverlayContextStartY = 0.f;
	float StartY = y - 13.f * Menu->Scale;// + 15.f * Menu->Scale;

	for (auto& Element : this->Elements) {
		
		if (Element->ShouldOverlay()) {
			Overlay = Element;
			OverlayContextStartY = StartY;
			break;
		}

		


	}

	bool Disable = false;

	for (auto& Element : this->Elements) {

	
		StartY += Element->GetOffset();

		if (Element->ShouldRender())
		{
			if (Element == Overlay) { 
				continue;
			}
			Element->Draw(x + 15.f * Menu->Scale, StartY, Size, MaxAlpha, Menu->MouseClick, Menu->MousePress, Disable);
		}
		
		//StartY += Element->GetOffset();
	}

	Render::PopClipRect();
}

inline bool Child::InRegion(float x, float y, float w, float h) {
	return Menu->MousePos.x > x && Menu->MousePos.y > y && Menu->MousePos.x < x + w && Menu->MousePos.y < y + h;
}
#pragma endregion 

float Switch::GetOffset() {
	return GUIAnimations::Ease(OffsetAnimation) * Menu->Scale * 28.f;
}
bool Switch::Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {
	float OriginalMaxAlpha = MaxAlpha;
	MaxAlpha *= GUIAnimations::Ease(OffsetAnimation);

	bool Hovered = Menu->InRegion(x + Size.x - 76.f * Menu->Scale, y - 6.f * Menu->Scale, 47.f * Menu->Scale, 30.f * Menu->Scale) && !disable && OffsetAnimation == 1.f;

	if ((Hovered && HoverAnimation < 1.f) || (!Hovered && HoverAnimation > 0.f))
		HoverAnimation = Math::Clamp(HoverAnimation + ((Hovered ? 1.f : -1.f) * 0.006f * Menu->AnimationModifier), 0.f, 1.f);

	if ((*Pointer && Slide < 1.f) || (!*Pointer && Slide > 0.f))
		Slide = Math::Clamp(Slide + ((*Pointer ? 1.f : -1.f) * 0.0061f * Menu->AnimationModifier), 0.f, 1.f);

	float EasedSlide = GUIAnimations::Ease(Slide);
	float W = 120 + 135 * EasedSlide; //slight optimization for more memory usage lol
	if (Hovered && LeftClick) {
		LeftClick = false;
		disable = true;
		*Pointer = !(*Pointer);
	}

	

	if (Render::TextSize(Fonts::MenuThin, Label.c_str()).x > Size.x - 103.f * Menu->Scale) {
		Render::PushClipRect(x, y - 4.f * Menu->Scale, Size.x - 103.f * Menu->Scale, 23.f * Menu->Scale, true);
		Render::DrawString(x, y, Col(W, W, W, MaxAlpha), Fonts::MenuThin, 0, Label.c_str());
		Col Full = Col(0, 1, 2, MaxAlpha);
		Col Low = Col(0, 1, 2, 0);
		//note: rendered twice for quadratic effect
		Render::GradientFilledRect(x + Size.x - 133.f * Menu->Scale, y - 4.f * Menu->Scale, 33.f * Menu->Scale, 26.f * Menu->Scale, Low, Full, Low, Full);
		Render::GradientFilledRect(x + Size.x - 133.f * Menu->Scale, y - 4.f * Menu->Scale, 33.f * Menu->Scale, 26.f * Menu->Scale, Low, Full, Low, Full);
		Render::GradientFilledRect(x + Size.x - 133.f * Menu->Scale, y - 4.f * Menu->Scale, 33.f * Menu->Scale, 26.f * Menu->Scale, Low, Full, Low, Full);
		Render::PopClipRect();
	}
	else
		Render::DrawString(x, y, Col(W, W, W, MaxAlpha), Fonts::MenuThin, 0, Label.c_str());

	Col Main(11 + 132 * EasedSlide, 12 + 138 * EasedSlide, 18 + 237 * EasedSlide, MaxAlpha);
	Col Background(0, 3, 6, MaxAlpha * 0.78f);	
	Render::FilledRoundedRect(x + Size.x - 69.f * Menu->Scale, y, 39.f * Menu->Scale, 16.f * Menu->Scale, Background, 20.f * Menu->Scale);
	if (HoverAnimation > 0) {
		Render::GradientCircle(x + Size.x - (62.5f - 25.f * EasedSlide * (OriginalMaxAlpha * 0.0039f)) * Menu->Scale, y + 8.5f * Menu->Scale, 17.f * Menu->Scale, Col(11 + 132 * EasedSlide * 0.43f, 12 + 138 * EasedSlide * 0.43f, 18 + 237 * EasedSlide * 0.43f, MaxAlpha * HoverAnimation * 0.55f), Col(11 + 132 * EasedSlide * 0.43f, 12 + 138 * EasedSlide * 0.43f, 18 + 237 * EasedSlide * 0.43f, 0), false);
	}
	Render::FilledCircle(x + Size.x - (62.5f - 25.f * EasedSlide * (OriginalMaxAlpha * 0.0039f)) * Menu->Scale, y + 8.5f * Menu->Scale, 10.f * Menu->Scale, Main, 17);

	


	return false;
}
bool Switch::ShouldRender() {

	if (ShouldRenderFn == nullptr)
		return true;

	bool ret = ShouldRenderFn();

	if ((ret && OffsetAnimation < 1.f) || (!ret && OffsetAnimation > 0.f))
		OffsetAnimation = Math::Clamp(OffsetAnimation + ((ret ? 1.f : -1.f) * 0.008f * Menu->AnimationModifier), 0.f, 1.f);

	return ret || OffsetAnimation > 0.f;
}
bool Switch::ShouldOverlay() {
	return false;
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

float ConfigView::GetOffset() {
	return 0.f;
}

void ConfigView::DrawConfig(float x, float y, float MaxAlpha, std::string label, ConfigView::ConfigAnimation& Context, bool& LeftClick, bool ShouldDoUpdate) {

	MaxAlpha *= Context.Offset;

	bool MenuHovered = Menu->InRegion(x, y, 193.f * Menu->Scale, 150.f * Menu->Scale) && !Context.ToBeDeleted && ShouldDoUpdate;
	bool DeleteHovered = Menu->InRegion(x, y + 130.f * Menu->Scale, 40.f * Menu->Scale, 40.f * Menu->Scale) && !Context.ToBeDeleted && ShouldDoUpdate && label != "Default";

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
	
	if(label != "Default")
		Render::DrawString(x + 20.f * Menu->Scale, y + 130.f * Menu->Scale, Col(55 + Context.Delete * 200, 55 + Context.Delete * 200, 55 + Context.Delete * 200, MaxAlpha * (0.5f + 0.5f * Context.Delete)), Fonts::MenuIcons, Render::centered_xy, "T");
}

bool ConfigView::Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {

	if (MaxAlpha != 255.f && (int(Menu->CurrentClock * 0.08f) % 2) == 0)
		Reload();

	Vec2 OrP	(x, y);
	Render::PushClipRect(x, y, Size.x - 40.f * Menu->Scale, Size.y, true);
	x += 17.f * Menu->Scale;
	y += 40.f * Menu->Scale;

	if ((SelectedConfig != "" && SelectionOpenAnimation < 1.f) || (SelectedConfig == "" && SelectionOpenAnimation > 0.f))
		SelectionOpenAnimation = Math::Clamp(SelectionOpenAnimation + ((SelectedConfig != "" ? 1.f : -1.f) * 0.007f * Menu->AnimationModifier), 0.f, 1.f);




	if (ConfigViews.size() > 6) {
		while (Client->ScrollAmmount > 0) {
			Scroll -= 15.f;
			Client->ScrollAmmount--;
		}
		while (Client->ScrollAmmount < 0) {
			Scroll += 15.f;
			Client->ScrollAmmount++;
		}
		Math::ClampPtr(Scroll, 0.f, 180.f * ConfigViews.size() * 0.33333333f);
	}
	else
		Scroll = 0;
	y -= Scroll;
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
		Render::FilledRoundedRect(x - 15.f * Menu->Scale, y + 13.f * Menu->Scale, Size.x, Size.y, Col(0, 1, 2, MaxAlpha), 4.5f * Menu->Scale);
		Render::DrawString(x + 25.f * Menu->Scale, y + 48.f * Menu->Scale, Col(130 + 125 * SelectionBackAnimation, 130 + 125 * SelectionBackAnimation, 130 + 125 * SelectionBackAnimation, MaxAlpha), Fonts::MenuIcons, Render::centered_xy, "M");

		Render::DrawString(x + 60.f * Menu->Scale, y + 90.f * Menu->Scale, Col(255, 255, 255, MaxAlpha), Fonts::MenuThin, 0, "Config: ");

		bool HoveredOverCfg = Menu->InRegion(x + 127.f * Menu->Scale, y + 84.f * Menu->Scale, 470.f * Menu->Scale, 29.f * Menu->Scale) && LastSelectedConfig != "Default";

		GUIAnimations::Animate(SelectionRenameAnimation, HoveredOverCfg || SelectionInRename);
		std::string WrittenCFG = EditableText;
		if (HoveredOverCfg) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
			if (LeftClick)
				SelectionInRename = true;

		}
		else if (LeftClick && SelectionInRename) {
			SelectionInRename = false;
			if (EditableText == "") {
				EditableText = LastSelectedConfig;
			}
			else {
				ConfigSystem->RebindConfig(LastSelectedConfig, EditableText);
				LastSelectedConfig = EditableText;
				SelectedConfig = EditableText;
				Reload();
			}
			
		}

		if (SelectionInRename) {
			ImGui::GetIO().WantTextInput = true;
			if ((int(Menu->CurrentClock * 0.0023f) % 2) == 0)
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
							LastSelectedConfig = EditableText;
							SelectedConfig = EditableText;
							Reload();
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
			Render::FilledRoundedRect(x + 529.f * Menu->Scale - TextSize1.x * 0.5f, y + 367.f * Menu->Scale - TextSize1.y * 0.5f, TextSize1.x + 22.f * Menu->Scale, TextSize1.y + 10.f * Menu->Scale, Col((138 + SelectionLoadAnimation * 30) * SelectionSaveAnimation, (138 + SelectionLoadAnimation * 30) * SelectionSaveAnimation, (242 + SelectionLoadAnimation * 13) * SelectionSaveAnimation, MaxAlpha), 6.f * Menu->Scale);
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