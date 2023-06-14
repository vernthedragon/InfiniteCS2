#include "Rendering.h"
#include "Menu.h"
#define NORMALIZE2F_OVER_ZERO(VX,VY)     { float d2 = VX*VX + VY*VY; if (d2 > 0.0f) { float inv_len = Math::InvSqrt(d2); VX *= inv_len; VY *= inv_len; } } (void)0
#define FIXNORMAL2F_MAX_INVLEN2          100.0f // 500.0f (see #4053, #3366)
#define FIXNORMAL2F(VX,VY)               { float d2 = VX*VX + VY*VY; if (d2 > 0.000001f) { float inv_len2 = 1.0f / d2; if (inv_len2 > FIXNORMAL2F_MAX_INVLEN2) inv_len2 = FIXNORMAL2F_MAX_INVLEN2; VX *= inv_len2; VY *= inv_len2; } } (void)0

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
void Render::FilledRect(float x, float y, float l, float w, Col color) {
	DrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + l, y + w), color.u32());
	//ImGui::GetBackgroundDrawList()->PushTextureID(Fonts::MenuMain->ContainerAtlas->TexID);
//	ImGui::PushFont(Fonts::MenuMain);


	//ImGui::GetBackgroundDrawList()->AddText(ImVec2(500,500), Col, "test text rend");
	//ImGui::PopFont();
	//THIS IS ONLY TESTING RENDERING IGNORE THIS HORRENDOUS CODE
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


void Render::DoRender(ID3D11Device* Device, ID3D11DeviceContext* Context, HWND Window, ID3D11RenderTargetView* RenderView) {

	DrawList = ImGui::GetBackgroundDrawList();

	Client->UpdateKeyStates();

	
	Menu->OnRender();


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
	//cfg.SizePixels = 13.f;

	io.Fonts->Clear();
	Fonts::MenuMain = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/NirmalaB.ttf", 22, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->Build();

	//build table used for shapes with a curve

	for (float i = 0.f; i <= SinCosPoints; i++)
		SinCosTable.emplace_back(
			std::cos(2.f * FPI * (i / static_cast<float>(SinCosPoints))),
			std::sin(2.f * FPI * (i / static_cast<float>(SinCosPoints)))
		);

	Initialized = true;
}

namespace Fonts {
	ImFont* MenuMain = nullptr;
};