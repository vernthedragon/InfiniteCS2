#pragma once

#include <cstdint>
#include "Vectors.h"

struct Bone
{
    const char* bone_name;
    const char* parent_name;
    char pad1[0x30];
    Vec3 center;
    Vec3 size;
    float radius;
};

static_assert(sizeof(Bone) == 0x60, "global_vars_t has wrong size");

struct RenderMesh_t
{
    char pad1[0xBC];
    Bone* skeleton;
} ;

static_assert(sizeof(RenderMesh_t) == 0xC8, "RenderMesh_t has wrong size");

struct RenderMeshes_t
{
    RenderMesh_t* meshes;
} ;

static_assert(sizeof(RenderMeshes_t) == 0x8, "RenderMeshes_t has wrong size");

class Model
{
public:
    char pad1[0x78];
    RenderMeshes_t* RenderMesh;
};

static_assert(sizeof(Model) == 0x80, "Model has wrong size");

class Skeleton
{
public:
    Model* GetModel()
    {
        auto ptr = *reinterpret_cast<void**>((uintptr_t)this + 0x200);

        if (!ptr)
            return nullptr;

        return *reinterpret_cast<Model**>(ptr);
    }
};