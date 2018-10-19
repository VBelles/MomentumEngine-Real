#pragma once


class CTransform;

struct TCompLOD : public TCompBase {
    VHandles handles;
    std::vector< std::string > names;
    AABB aabb;

    bool  children_dynamic = false;
    bool  aabb_update_required = true;

    float lod_level = 0.f;
    float lod_threshold = 0.5f;
    bool  using_lod = false;

    float distance_to_camera = 1.f;
    float my_size = 1.f;

    std::string camera_name;
    CHandle     h_camera;

	static void registerMsgs();
	void onGroupCreated(const TMsgEntitiesGroupCreated & msg);

    void debugInMenu();
    void update(float delta);
    void load(const json& j, TEntityParseContext& ctx);
    void updateLevel();
    void updateAABBFromChildren();
    void applyLODStatus();

    DECL_SIBLING_ACCESS();
};
