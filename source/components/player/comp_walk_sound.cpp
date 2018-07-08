#include "mcv_platform.h"
#include "comp_walk_sound.h"
#include "components/comp_transform.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/StateManager.h"
#include "components/player/states/IActionState.h"
#include "components/player/comp_power_gauge.h"
#include "skeleton/comp_skeleton.h"
#include "skeleton/cal3d2engine.h"
#include "entity/entity_parser.h"

DECL_OBJ_MANAGER("walk_sound", TCompWalkSound);

void TCompWalkSound::registerMsgs() {
	DECL_MSG(TCompWalkSound, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompWalkSound::load(const json& j, TEntityParseContext& ctx) {
	for (auto& jFeetConfig : j["feet"]) {
		std::string boneName = jFeetConfig.value("bone", "");
		assert(boneName != "");
		std::string eventName = jFeetConfig.value("event", "");
		assert(eventName != "");
		feetConfig.push_back(FootConfig{ boneName, eventName });
	}
}

void TCompWalkSound::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	skeletonHandle = get<TCompSkeleton>();
	transformHandle = get<TCompTransform>();
	playerModelHandle = get<TCompPlayerModel>();
	assert(skeletonHandle.isValid());
	for (auto& footConfig : feetConfig) {
		int boneId = getSkeleton()->model->getCoreModel()->getCoreSkeleton()->getCoreBoneId(footConfig.boneName);
		auto bone = getSkeleton()->model->getSkeleton()->getBone(boneId);
		FootInfo info;
		info.boneId = boneId;
		info.eventName = footConfig.eventName;
		feetInfo.push_back(info);
	}
}

void TCompWalkSound::update(float delta) {
	for (auto& footInfo : feetInfo) {
		auto bone = getSkeleton()->model->getSkeleton()->getBone(footInfo.boneId);
		VEC3 pos = Cal2DX(bone->getTranslationAbsolute()) - getTransform()->getPosition();

		float yDist = pos.y - footInfo.prevPos.y;
		footInfo.yTravel += abs(yDist);

		if (yDist > footInfo.yThreshold) {
			footInfo.goingUp = true;
		}
		else if (yDist < footInfo.yThreshold) {
			if (footInfo.goingUp) {
				footInfo.canEmitSound = true;
			}
			footInfo.goingUp = false;
		}

		if (footInfo.canEmitSound && pos.y <= 0.2f) {
			footInfo.canEmitSound = false;
			emitSound(footInfo);
		}

		footInfo.prevPos = pos;

	}
}


void TCompWalkSound::emitSound(const FootInfo& footInfo) {
	IActionState* state = getPlayerModel()->getStateManager()->getState();
	if (((state->state == State::Run && getPlayerModel()->getPowerGauge()->getPowerLevel() != 3) 
			|| state->state == State::Walk)
		&& state->getMovementInput().Length() > 0) {
		EngineSound.emitEvent(footInfo.eventName);
	}
}

TCompTransform* TCompWalkSound::getTransform() {
	return transformHandle;
}

TCompSkeleton* TCompWalkSound::getSkeleton() {
	return skeletonHandle;
}

TCompPlayerModel* TCompWalkSound::getPlayerModel() {
	return playerModelHandle;
}


