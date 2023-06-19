#include "Entities.h"
#include "Client.h"
#include "Interfaces.h"
bool IEntity::IsBasePlayerController() {
	return CallVFunc<bool>(this, 141);
}
bool IEntity::IsBasePlayerWeapon() {
	return CallVFunc<bool>(this, 147);
}
bool IEntity::IsChicken() {
    schema_class_info_data_t* pClassInfo = Schema_DynamicBinding();
    if (!pClassInfo) return false;

    const char* className = pClassInfo->GetName();
    if (!className) return false;


    return (className) == "C_Chicken";
}
bool IEntity::IsViewModel() {
    return CallVFunc<bool>(this, 238);
}

const Vec3& IEntity::GetOrigin() {
    static const Vec3 null(0.f,0.f,0.f);

    IGameSceneNode* pGameSceneNode = m_pGameSceneNode();
    if (!pGameSceneNode) return null;

    return pGameSceneNode->m_vecAbsOrigin();
}
bool IEntity::GetBoundingBox(BoundingBox& out) {
	Vec3 origin = this->m_pGameSceneNode()->m_vecAbsOrigin();

	Vec2 flb, brt, blb, frt, frb, brb, blt, flt = { };
	float left, top, right, bottom = 0.f;

	Vec3 min = this->m_pCollision()->m_vecMins() + origin;
	Vec3 max = this->m_pCollision()->m_vecMaxs() + origin;

	std::array<Vec3, 8> points = {
		Vec3(min.x, min.y, min.z),
		Vec3(min.x, min.y, max.z),
		Vec3(min.x, max.y, min.z),
		Vec3(min.x, max.y, max.z),
		Vec3(max.x, min.y, min.z),
		Vec3(max.x, min.y, max.z),
		Vec3(max.x, max.y, min.z),
		Vec3(max.x, max.y, max.z) };

	if (!Vec2::WorldToScreen(points[3], flb)
		|| !Vec2::WorldToScreen(points[5], brt)
		|| !Vec2::WorldToScreen(points[0], blb)
		|| !Vec2::WorldToScreen(points[4], frt)
		|| !Vec2::WorldToScreen(points[2], frb)
		|| !Vec2::WorldToScreen(points[1], brb)
		|| !Vec2::WorldToScreen(points[6], blt)
		|| !Vec2::WorldToScreen(points[7], flt))
	{
		return false;
	}

	std::array<Vec2, 8> arr = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (std::int32_t i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
		{
			left = arr[i].x;
		}

		if (bottom < arr[i].y)
		{
			bottom = arr[i].y;
		}

		if (right < arr[i].x)
		{
			right = arr[i].x;
		}

		if (top > arr[i].y)
		{
			top = arr[i].y;
		}
	}

	out.x = left;
	out.y = top;
	out.w = right - left;
	out.h = bottom - top;
	return true;
}
bool IEntity::ComputeHitboxSurroundingBox(Vec3& mins, Vec3& maxs) {
    using fn = bool(__fastcall* )(void*, Vec3&,
        Vec3&);

    static fn ComputeHitboxSurroundingBoxF = UTILPtr(Client->FindPattern(GetModuleHandleA("client.dll"), COMPUTE_HITBOX_SURROUNDING_BOX)).toAbsolute(1, 0).get<fn>();
    return ComputeHitboxSurroundingBoxF(this, mins, maxs);
}
float IEntity::DistanceToSquared(IEntity* pEntity) {
    return 0.f;
}