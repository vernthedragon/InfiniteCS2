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
bool IEntity::GetBoundingBox(BoundingBox& out, bool computeSurroundingBox) {
    return false;
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