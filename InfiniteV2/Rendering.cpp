#include "Rendering.h"
#include "Menu.h"
#include "ByteData.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB_Image.h"
#define NORMALIZE2F_OVER_ZERO(VX,VY)     { float d2 = VX*VX + VY*VY; if (d2 > 0.0f) { float inv_len = Math::InvSqrt(d2); VX *= inv_len; VY *= inv_len; } } (void)0
#define FIXNORMAL2F_MAX_INVLEN2          100.0f // 500.0f (see #4053, #3366)
#define FIXNORMAL2F(VX,VY)               { float d2 = VX*VX + VY*VY; if (d2 > 0.000001f) { float inv_len2 = 1.0f / d2; if (inv_len2 > FIXNORMAL2F_MAX_INVLEN2) inv_len2 = FIXNORMAL2F_MAX_INVLEN2; VX *= inv_len2; VY *= inv_len2; } } (void)0

//function unsafe can cause crash
bool Render::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	Hooks::Device->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	Hooks::Device->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();


	stbi_image_free(image_data);

	return true;
}
void Render::PushClipRect(float x, float y, float w, float h, bool IntersectWithCurrentClipRect) {
	DrawList->PushClipRect(ImVec2(x, y), ImVec2(x + w, y + h), IntersectWithCurrentClipRect);
}
void Render::PopClipRect() {
	DrawList->PopClipRect();
}
void Render::DrawVertexes(Vertex* Vertices, int Count, bool antialiased ) {
	if (Count < 3)
		return;

	const ImVec2 uv = DrawList->_Data->TexUvWhitePixel;

	if (antialiased)
	{
		// Anti-aliased Fill
		const float AA_SIZE = DrawList->_FringeScale;
		
		const int idx_count = (Count - 2) * 3 + Count * 6;
		const int vtx_count = (Count * 2);
		DrawList->PrimReserve(idx_count, vtx_count);

		// Add indexes for fill
		unsigned int vtx_inner_idx = DrawList->_VtxCurrentIdx;
		unsigned int vtx_outer_idx = DrawList->_VtxCurrentIdx + 1;
		for (int i = 2; i < Count; i++)
		{
			DrawList->_IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx); DrawList->_IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx + ((i - 1) << 1)); DrawList->_IdxWritePtr[2] = (ImDrawIdx)(vtx_inner_idx + (i << 1));
			DrawList->_IdxWritePtr += 3;
		}

		// Compute normals
		DrawList->_Data->TempBuffer.reserve_discard(Count);
		ImVec2* temp_normals = DrawList->_Data->TempBuffer.Data;
		for (int i0 = Count - 1, i1 = 0; i1 < Count; i0 = i1++)
		{
			const ImVec2& p0 = ImVec2(Vertices[i0].x, Vertices[i0].y);
			const ImVec2& p1 = ImVec2(Vertices[i1].x, Vertices[i1].y);
			float dx = p1.x - p0.x;
			float dy = p1.y - p0.y;
			NORMALIZE2F_OVER_ZERO(dx, dy);
			temp_normals[i0].x = dy;
			temp_normals[i0].y = -dx;
		}

		for (int i0 = Count - 1, i1 = 0; i1 < Count; i0 = i1++)
		{
			const ImU32 col_trans = Vertices[i1].Color & ~IM_COL32_A_MASK;
			// Average normals
			const ImVec2& n0 = temp_normals[i0];
			const ImVec2& n1 = temp_normals[i1];
			float dm_x = (n0.x + n1.x) * 0.5f;
			float dm_y = (n0.y + n1.y) * 0.5f;
			FIXNORMAL2F(dm_x, dm_y);
			dm_x *= AA_SIZE * 0.5f;
			dm_y *= AA_SIZE * 0.5f;

			// Add vertices
			DrawList->_VtxWritePtr[0].pos.x = (Vertices[i1].x - dm_x); DrawList->_VtxWritePtr[0].pos.y = (Vertices[i1].y - dm_y); DrawList->_VtxWritePtr[0].uv = uv; DrawList->_VtxWritePtr[0].col = Vertices[i1].Color;        // Inner
			DrawList->_VtxWritePtr[1].pos.x = (Vertices[i1].x + dm_x); DrawList->_VtxWritePtr[1].pos.y = (Vertices[i1].y + dm_y); DrawList->_VtxWritePtr[1].uv = uv; DrawList->_VtxWritePtr[1].col = col_trans;  // Outer
			DrawList->_VtxWritePtr += 2;

			// Add indexes for fringes
			DrawList->_IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx + (i1 << 1)); DrawList->_IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx + (i0 << 1)); DrawList->_IdxWritePtr[2] = (ImDrawIdx)(vtx_outer_idx + (i0 << 1));
			DrawList->_IdxWritePtr[3] = (ImDrawIdx)(vtx_outer_idx + (i0 << 1)); DrawList->_IdxWritePtr[4] = (ImDrawIdx)(vtx_outer_idx + (i1 << 1)); DrawList->_IdxWritePtr[5] = (ImDrawIdx)(vtx_inner_idx + (i1 << 1));
			DrawList->_IdxWritePtr += 6;
		}
		DrawList->_VtxCurrentIdx += (ImDrawIdx)vtx_count;
	}
	else
	{

		const int idx_count = (Count - 2) * 3;
		const int vtx_count = Count;
		DrawList->PrimReserve(idx_count, vtx_count);
		for (int i = 0; i < vtx_count; i++)
		{
			DrawList->_VtxWritePtr[0].pos = ImVec2(Vertices[i].x, Vertices[i].y);  DrawList->_VtxWritePtr[0].uv = uv;  DrawList->_VtxWritePtr[0].col = Vertices[i].Color;
			DrawList->_VtxWritePtr++;
		}
		for (int i = 2; i < Count; i++)
		{
			DrawList->_IdxWritePtr[0] = (ImDrawIdx)(DrawList->_VtxCurrentIdx);  DrawList->_IdxWritePtr[1] = (ImDrawIdx)(DrawList->_VtxCurrentIdx + i - 1);  DrawList->_IdxWritePtr[2] = (ImDrawIdx)(DrawList->_VtxCurrentIdx + i);
			DrawList->_IdxWritePtr += 3;
		}
		DrawList->_VtxCurrentIdx += (ImDrawIdx)vtx_count;
	}
}
void Render::DrawString(float x, float y, Col color, ImFont* font, unsigned int flags, const char* message) {
	


	DrawList->PushTextureID(font->ContainerAtlas->TexID);
	ImGui::PushFont(font);

	auto size = ImGui::CalcTextSize(message);





	if (!(flags & Render::centered_x))
		size.x = 0;
	if (!(flags & Render::centered_y))
		size.y = 0;

	ImVec2 pos = ImVec2(x - (size.x * .5), y - (size.y * .5));

	if (flags & Render::outline)
	{
		Col outline_clr = Col(0, 0, 0, color[3]);
		pos.y++;
		DrawList->AddText(pos, outline_clr.u32(), message);
		pos.x++;
		DrawList->AddText(pos, outline_clr.u32(), message);
		pos.y--;
		DrawList->AddText(pos, outline_clr.u32(), message);
		pos.x--;
		DrawList->AddText(pos, outline_clr.u32(), message);
	}

	DrawList->AddText(pos, color.u32(), message);
	ImGui::PopFont();
}
Vec2 Render::TextSizeFmt(ImFont* font, const char* message, ...) {
	char output[1024] = {};
	va_list args;
	va_start(args, message);
	vsprintf_s(output, message, args);
	va_end(args);
	ImGui::PushFont(font);
	auto size = ImGui::CalcTextSize(output);
	ImGui::PopFont();
	return Vec2(size.x, size.y);
}
Vec2 Render::TextSize(ImFont* font, const char* message) {
	
	ImGui::PushFont(font);
	auto size = ImGui::CalcTextSize(message);
	ImGui::PopFont();
	return Vec2(size.x, size.y);
}
void Render::DrawFullscreenBlur() {

}
void Render::DrawStringFmt(float x, float y, Col color, ImFont* font, unsigned int flags, const char* message, ...) {

	char output[1024] = {};
	va_list args;
	va_start(args, message);
	vsprintf_s(output, message, args);
	va_end(args);


	DrawList->PushTextureID(font->ContainerAtlas->TexID);
	ImGui::PushFont(font);

	auto size = ImGui::CalcTextSize(output);





	if (!(flags & Render::centered_x))
		size.x = 0;
	if (!(flags & Render::centered_y))
		size.y = 0;

	ImVec2 pos = ImVec2(x - (size.x * .5), y - (size.y * .5));

	if (flags & Render::outline)
	{
		Col outline_clr = Col(0, 0, 0, color[3]);
		pos.y++;
		DrawList->AddText(pos, outline_clr.u32(), output);
		pos.x++;
		DrawList->AddText(pos, outline_clr.u32(), output);
		pos.y--;
		DrawList->AddText(pos, outline_clr.u32(), output);
		pos.x--;
		DrawList->AddText(pos, outline_clr.u32(), output);
	}

	DrawList->AddText(pos, color.u32(), output);
	ImGui::PopFont();
}
void Render::FilledRoundedRect(float x, float y, float l, float w, Col color, float rounding) {
	DrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + l, y + w), color.u32(), rounding, ImDrawFlags_RoundCornersAll);
}
void Render::FilledCircle(float x, float y, float r, Col color, int count){
	DrawList->AddCircleFilled(ImVec2(x, y), r, color.u32(), count);
}
void Render::FilledRoundedRectCustom(float x, float y, float l, float w, Col color, float rounding,  int flags) {
	DrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + l, y + w), color.u32(), rounding, flags);
}
void Render::Rect(float x, float y, float l, float w, Col color, float thickness) {
	DrawList->AddRect(ImVec2(x, y), ImVec2(x + l, y + w), color.u32(), 0.f,0,thickness);
}
void Render::RoundedRect(float x, float y, float l, float w, Col color, float thickness, float rounding) {
	DrawList->AddRect(ImVec2(x, y), ImVec2(x + l, y + w), color.u32(), rounding, ImDrawFlags_RoundCornersAll, thickness);
}
void  Render::DrawTexture(float x, float y, float l, float w, void* text, Col color) {
	DrawList->AddImage(text, ImVec2(x, y), ImVec2(x + l, y + w), ImVec2(0, 0), ImVec2(1, 1), color.u32());
}
void  Render::DrawRoundedTexture(float x, float y, float l, float w , void* text, float rounding, Col color ) {
	DrawList->AddImageRounded(text, ImVec2(x, y), ImVec2(x + l, y + w), ImVec2(0,0), ImVec2(1,1), color.u32(),rounding, ImDrawFlags_::ImDrawFlags_RoundCornersAll);
}

void Render::FilledRect(float x, float y, float l, float w, Col color) {
	DrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + l, y + w), color.u32());
	//ImGui::GetBackgroundDrawList()->PushTextureID(Fonts::MenuMain->ContainerAtlas->TexID);
//	ImGui::PushFont(Fonts::MenuMain);


	//ImGui::GetBackgroundDrawList()->AddText(ImVec2(500,500), Col, "test text rend");
	//ImGui::PopFont();
	//THIS IS ONLY TESTING RENDERING IGNORE THIS HORRENDOUS CODE
}
void AddRectMultiColor(const ImVec2& p_min, const ImVec2& p_max, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left)
{
	if (((col_upr_left | col_upr_right | col_bot_right | col_bot_left) & IM_COL32_A_MASK) == 0)
		return;

	const ImVec2 uv = Render::DrawList->_Data->TexUvWhitePixel;
	Render::DrawList->PrimReserve(6, 4);
	Render::DrawList->PrimWriteIdx((ImDrawIdx)(Render::DrawList->_VtxCurrentIdx)); Render::DrawList->PrimWriteIdx((ImDrawIdx)(Render::DrawList->_VtxCurrentIdx + 1)); Render::DrawList->PrimWriteIdx((ImDrawIdx)(Render::DrawList->_VtxCurrentIdx + 2));
	Render::DrawList->PrimWriteIdx((ImDrawIdx)(Render::DrawList->_VtxCurrentIdx)); Render::DrawList->PrimWriteIdx((ImDrawIdx)(Render::DrawList->_VtxCurrentIdx + 2)); Render::DrawList->PrimWriteIdx((ImDrawIdx)(Render::DrawList->_VtxCurrentIdx + 3));
	Render::DrawList->PrimWriteVtx(p_min, uv, col_upr_left);
	Render::DrawList->PrimWriteVtx(ImVec2(p_max.x, p_min.y), uv, col_upr_right);
	Render::DrawList->PrimWriteVtx(p_max, uv, col_bot_right);
	Render::DrawList->PrimWriteVtx(ImVec2(p_min.x, p_max.y), uv, col_bot_left);
}
void Render::GradientFilledRect(float x, float y, float l, float w, Col left, Col right, Col bl, Col br) {
	AddRectMultiColor(ImVec2(x, y), ImVec2(x + l, y + w),
		left.u32(),right.u32(),br.u32(),bl.u32());
}
void Render::GradientCircle(float x, float y, float radius, Col inner, Col outer, bool antialiased) {
	Vertex vert[SinCosPoints + 2] = {};
	ImU32 out = outer.u32();

	for (auto i = 1; i <= SinCosPoints; i++)
		vert[i] =
	{
		x + radius * SinCosTable[i].x,
		y - radius * SinCosTable[i].y,
		out
	};

	vert[0] = { x,y, inner.u32() };
	vert[SinCosPoints + 1] = vert[1];

	DrawVertexes(vert, SinCosPoints + 2, antialiased);
}
void Render::FilledTriangle(float x1, float y1, float x2, float y2, float x3, float y3, Col clr)
{
	DrawList->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), clr.u32());
}


void  Render::Line(float x, float y, float endx, float endy, Col clr, float thick) {
	Render::DrawList->AddLine(ImVec2(x,y), ImVec2(endx, endy), clr.u32(), thick);
}
#include "PlayerHandler.h"
void Render::DoRender(ID3D11Device* Device, ID3D11DeviceContext* Context, HWND Window, ID3D11RenderTargetView* RenderView) {

	DrawList = ImGui::GetBackgroundDrawList();

	Client->UpdateKeyStates();

	//Render::UpdateProjectionMatrix();
	PlayerHandler->UpdateRender();
	Menu->OnRender();


}
void Render::UpdateProjectionMatrix() {
	static Mat4x4* ProjectionMatrixPtr =(Mat4x4*)Client->FindPattern(GetModuleHandleA("client.dll"), WORLD_TO_SCREEN_PROJECTION_MATRIX);
	ProjectionMatrix = *ProjectionMatrixPtr;
}
/*
bool Math::screen_transform(const Vector& in, Vector& out)
{
	const auto& Render::ProjectionMatrix = world_to_screen_matrix();
	out.x = Render::ProjectionMatrix[0][0] * x + Render::ProjectionMatrix[0][1] * in[1] + Render::ProjectionMatrix[0][2] * in[2] + Render::ProjectionMatrix[0][3];
	out.y = Render::ProjectionMatrix[1][0] * x + Render::ProjectionMatrix[1][1] * in[1] + Render::ProjectionMatrix[1][2] * in[2] + Render::ProjectionMatrix[1][3];
	out.z = 0.0f;

	const auto w = Render::ProjectionMatrix[3][0] * in.x + Render::ProjectionMatrix[3][1] * in.y + Render::ProjectionMatrix[3][2] * in.z + Render::ProjectionMatrix[3][3];

	if (w < 0.001f)
	{
		out.x *= 100000;
		out.y *= 100000;
		return false;
	}

	const auto invw = 1.0f / w;
	out.x *= invw;
	out.y *= invw;

	return true;
}
*/
bool Vec2::WorldToScreen(Vec3& Vec, Vec2& Out) {
	using function_t = bool(__fastcall*)(Vec3&, Vec3&);
	static function_t function = reinterpret_cast<function_t>(Client->FindPattern(GetModuleHandleA("client.dll"), WORLD_TO_SCREEN));

	Vec3 out = { };
	if (function(Vec, out))
	{
		return false;
	}
	Out.x = (1.0f + out.x) * (Client->ScreenSize.x * 0.5f);
	Out.y = (1.0f - out.y) * (Client->ScreenSize.y * 0.5f);
	return true;
}
Vec2 Vec3::ToScreen() //check success with OnScreen()
{
	using function_t = bool(__fastcall*)(Vec3&, Vec3&);
	static function_t function = reinterpret_cast<function_t>(Client->FindPattern(GetModuleHandleA("client.dll"), WORLD_TO_SCREEN));

	Vec3 out = { };
	if (function(*this, out))
	{
		return Vec2(-1337.f, -1337.f);
	}
	Vec2 screen;
	screen.x = (1.0f + out.x) * (Client->ScreenSize.x * 0.5f);
	screen.y = (1.0f - out.y) * (Client->ScreenSize.y * 0.5f);
	return screen;
	/*
	Render::UpdateProjectionMatrix();
	
	Vec2 Return(-1337.f, -1337.f);
	const auto wh = Render::ProjectionMatrix[3][0] * x + Render::ProjectionMatrix[3][1] * y + Render::ProjectionMatrix[3][2] * z + Render::ProjectionMatrix[3][3];
	
	if (wh < 0.001f)
	{
		return Return;
	}
	Return.x = Render::ProjectionMatrix[0][0] * x + Render::ProjectionMatrix[0][1] * y + Render::ProjectionMatrix[0][2] * z + Render::ProjectionMatrix[0][3];
	Return.y = Render::ProjectionMatrix[1][0] * x + Render::ProjectionMatrix[1][1] * y + Render::ProjectionMatrix[1][2] * z + Render::ProjectionMatrix[1][3];
	const auto invw = 1.0f / wh;
	Return.x *= invw;
	Return.y *= invw;

	int32_t w = 0;
	int32_t h = 0;
	g_Engine->GetScreenSize(w, h);

	Return.x = (w / 2.0f) + (Return.x * w) / 2.0f;
	Return.y = (h / 2.0f) - (Return.y * h) / 2.0f;

	return Return;*/
}
void Render::Initialize() {
	if (Initialized)
		return;
	 // build fonts
	ImGuiIO& io = ImGui::GetIO();

	ImVector<ImWchar> ranges;
	ImFontGlyphRangesBuilder builder;

	

	builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
	builder.BuildRanges(&ranges);

	ImFontConfig cfg{};
	cfg.OversampleH = 2.f;
	

	io.Fonts->Clear();
	Fonts::MenuMain50 = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/NirmalaB.ttf", 28.f * 0.5f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	Fonts::MenuIcons50 = io.Fonts->AddFontFromMemoryTTF(FontIcons, FontIconsLength, 20.f * 0.5f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	Fonts::MenuThin50 = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/corbel.ttf", 18.f * 0.5f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

	Fonts::MenuMain80 = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/NirmalaB.ttf", 28.f * 0.8f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	Fonts::MenuIcons80 = io.Fonts->AddFontFromMemoryTTF(FontIcons, FontIconsLength, 20.f * 0.8f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	Fonts::MenuThin80 = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/corbel.ttf", 18.f * 0.8f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

	Fonts::MenuMain100 = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/NirmalaB.ttf", 28.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	Fonts::MenuIcons100 = io.Fonts->AddFontFromMemoryTTF(FontIcons, FontIconsLength, 20.f, &cfg, io.Fonts->GetGlyphRangesCyrillic()); //20 = 100%
	Fonts::MenuThin100 = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/corbel.ttf", 18.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

	Fonts::MenuMain140 = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/NirmalaB.ttf", 28.f * 1.4f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	Fonts::MenuIcons140 = io.Fonts->AddFontFromMemoryTTF(FontIcons, FontIconsLength, 20.f * 1.4f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	Fonts::MenuThin140 = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/corbel.ttf", 18.f * 1.4f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

	Fonts::MenuMain170 = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/NirmalaB.ttf", 28.f * 1.7f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	Fonts::MenuIcons170 = io.Fonts->AddFontFromMemoryTTF(FontIcons, FontIconsLength, 20.f * 1.7f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	Fonts::MenuThin170 = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/corbel.ttf", 18.f * 1.7f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	//HERE WE SET THE DEFAULTS ALL TO 1
	Fonts::MenuMain = Fonts::MenuMain100;
	Fonts::MenuIcons = Fonts::MenuIcons100;
	Fonts::MenuThin = Fonts::MenuThin100;
	io.Fonts->Build();

	//build table used for shapes with a curve
	if (SinCosTable.empty()) {
		for (float i = 0.f; i <= SinCosPoints; i++)
			SinCosTable.emplace_back(
				std::cos(2.f * FPI * (i / static_cast<float>(SinCosPoints))),
				std::sin(2.f * FPI * (i / static_cast<float>(SinCosPoints)))
			);
	}

	
	Initialized = true;
}

namespace Fonts {
	ImFont* MenuMain = nullptr;
	ImFont* MenuIcons = nullptr;
	ImFont* MenuMain50 = nullptr;
	ImFont* MenuIcons50 = nullptr;
	ImFont* MenuMain80 = nullptr;
	ImFont* MenuIcons80 = nullptr;
	ImFont* MenuMain100 = nullptr;
	ImFont* MenuIcons100 = nullptr;
	ImFont* MenuMain140 = nullptr;
	ImFont* MenuIcons140 = nullptr;
	ImFont* MenuMain170 = nullptr;
	ImFont* MenuIcons170 = nullptr;

	ImFont* MenuThin = nullptr;
	ImFont* MenuThin50 = nullptr;
	ImFont* MenuThin80 = nullptr;
	ImFont* MenuThin100 = nullptr;
	ImFont* MenuThin140 = nullptr;
	ImFont* MenuThin170 = nullptr;
};