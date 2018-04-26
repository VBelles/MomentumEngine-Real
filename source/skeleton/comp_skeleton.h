#ifndef INC_COMP_SKELETON_H_
#define INC_COMP_SKELETON_H_

#include "components/comp_base.h"
#include "render/cte_buffer.h"
#include "ctes.h"

class CalModel;

class TCompSkeleton : public TCompBase {
	DECL_SIBLING_ACCESS();
public:
	CRenderCte<CCteSkinBones> cb_bones;
	CalModel* model = nullptr;
	TCompSkeleton();
	~TCompSkeleton();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void debugInMenu();
	void renderDebug();
	void updateCtesBones();

	void blendCycle(int animationId, float in_delay = 0.0f, float out_delay = 0.0f, float weight = 1.f, bool clearPrevious = true);
	void blendCycle(std::string animation, float in_delay = 0.0f, float out_delay = 0.0f, float weight = 1.f, bool clearPrevious = true);
	void executeAction(int animationId, float in_delay = 0.0f, float out_delay = 0.0f, float weight = 1.0f, bool auto_lock = false);
	void executeAction(std::string animation, float in_delay = 0.0f, float out_delay = 0.0f, float weight = 1.0f, bool auto_lock = false);
	void setTimeFactor(float timeFactor);
	float getAnimationTime();
	float getAnimationDuration(int animationId);
};

#endif
