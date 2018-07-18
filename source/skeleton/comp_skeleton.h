#pragma once

#include "components/comp_base.h"
#include "render/cte_buffer.h"
#include "ctes.h"

class CalModel;

class TCompSkeleton : public TCompBase {
private:
	bool stopped = false;
public:
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

	void blendCycle(int animationId, float in_delay = 0.0f, float out_delay = 0.0f, float weight = 1.f, bool clearPrevious = true);
	void blendCycle(std::string animation, float in_delay = 0.0f, float out_delay = 0.0f, float weight = 1.f, bool clearPrevious = true);
	void executeAction(int animationId, float in_delay = 0.0f, float out_delay = 0.0f, float weight = 1.0f, bool auto_lock = false);
	void executeAction(std::string animation, float in_delay = 0.0f, float out_delay = 0.0f, float weight = 1.0f, bool auto_lock = false);
	void removeAction(int animationId, float delay = 0.0f);
	void removeAction(std::string animation, float delay = 0.0f);
	void clear(float delay = 0.0f);
	void setTimeFactor(float timeFactor);
	float getAnimationTime();
	float getAnimationDuration(int animationId);
	float getAnimationDuration(std::string animation);

	int getBoneId(std::string bone);
	CalBone* getBone(int id);

	void stop();
	void resume();
};
