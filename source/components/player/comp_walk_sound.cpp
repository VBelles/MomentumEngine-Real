#include "mcv_platform.h"
#include "comp_walk_sound.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "skeleton/cal3d2engine.h"
#include "entity/entity_parser.h"

DECL_OBJ_MANAGER("walk_sound", TCompWalkSound);

void TCompWalkSound::registerMsgs() {
	DECL_MSG(TCompWalkSound, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompWalkSound::load(const json& j, TEntityParseContext& ctx) {
	for (const std::string& boneName : j["bones"]) {
		bonesName.push_back(boneName);
	}
}

void TCompWalkSound::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	skeletonHandle = get<TCompSkeleton>();
	transformHandle = get<TCompTransform>();
	assert(skeletonHandle.isValid());
	for (auto& boneName : bonesName) {
		int boneId = getSkeleton()->model->getCoreModel()->getCoreSkeleton()->getCoreBoneId(boneName);
		auto bone = getSkeleton()->model->getSkeleton()->getBone(boneId);

		VEC3 pos = Cal2DX(bone->getTranslationAbsolute()) - getTransform()->getPosition();
		feetInfo.push_back(FootInfo{boneId});
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
		else if (yDist <  footInfo.yThreshold) {
			if (footInfo.goingUp) {
				footInfo.canEmitSound = true;
			}
			footInfo.goingUp = false;
		}
	
		if (footInfo.canEmitSound && pos.y <= 0.2f) {
			dbg("Emit sound\n");
			dbg("%f %f %f\n", pos.x, pos.y, pos.z);
			footInfo.canEmitSound = false;
		}

		footInfo.prevPos = pos;

	}
}

TCompTransform* TCompWalkSound::getTransform() {
	return transformHandle;
}

TCompSkeleton* TCompWalkSound::getSkeleton() {
	return skeletonHandle;
}


