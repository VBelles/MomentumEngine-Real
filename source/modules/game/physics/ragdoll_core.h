#pragma once

struct TRagdollBoneCore {
    std::string bone;
    std::string parent_bone;
    float height;
    float radius;
    TRagdollBoneCore* parent_core;
    int instance_idx;
};

struct TRagdollCore {
    std::vector<TRagdollBoneCore> ragdoll_bone_cores;
};
