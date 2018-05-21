#pragma once

#include "resources/resource.h"
#include "cal3d/cal3d.h"
#include "bone_correction.h"
#include "modules/game/physics/ragdoll_core.h"

// This is the CalCoreModel + IResource interface + other metadata
// we might need in the future and is shared between all instances
// of this model
class CGameCoreSkeleton : public IResource, public CalCoreModel {
    std::string root_path;

    bool convertCalCoreMesh2RenderMesh(CalCoreMesh* cal_mesh, const std::string& ofilename);

public:
    CGameCoreSkeleton(const std::string& aname) : CalCoreModel(aname) {}
    bool create(const std::string& res_name);
    void debugInMenu() override;
    float              bone_ids_debug_scale = 1.f;
    std::vector< int > bone_ids_to_debug;

    std::vector< TBoneCorrection > lookat_corrections;

    TRagdollCore ragdoll_core;
};
