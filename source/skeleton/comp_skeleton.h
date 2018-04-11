#ifndef INC_COMP_SKELETON_H_
#define INC_COMP_SKELETON_H_

#include "../components/comp_base.h"
#include "render/cte_buffer.h"
#include "ctes.h"

class CalModel;

struct TCompSkeleton : public TCompBase {
	DECL_SIBLING_ACCESS();

	CRenderCte<CCteSkinBones> cb_bones;
	CalModel* model = nullptr;

	TCompSkeleton();
	~TCompSkeleton();

	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void debugInMenu();
	void renderDebug();
	void updateCtesBones();
};

#endif
