#include "mcv_platform.h"
#include "comp_transform.h"
#include "entity/entity_parser.h"
#include "render/render_objects.h"
#include "components/comp_collider.h"

DECL_OBJ_MANAGER("transform", TCompTransform);

void TCompTransform::debugInMenu() {
	bool changed = CTransform::debugInMenu();
	if (changed) {
		updateCollider();
	}
}

void TCompTransform::renderDebug() {
	setWorldTransform(asMatrix());
	auto axis = Resources.get("axis.mesh")->as<CRenderMesh>();
	axis->activateAndRender();
}

void TCompTransform::set(const CTransform& new_tmx) {
	*(CTransform*)this = new_tmx;
}

void TCompTransform::load(const json& j, TEntityParseContext& ctx) {
	CTransform::load(j);
	set(ctx.root_transform.combineWith(*this));
}

void TCompTransform::updateCollider() {
	CEntity* entity = CHandle(this).getOwner();
	CHandle colliderHandle = entity->get<TCompCollider>();
	if (colliderHandle.isValid()) {
		TCompCollider* compCollider = colliderHandle;
		if (compCollider->actor->getType() == PxActorType::eRIGID_DYNAMIC) {
			PxRigidDynamic* rd = static_cast<PxRigidDynamic*>(compCollider->actor);
			if (rd->getRigidBodyFlags().isSet(PxRigidBodyFlag::eKINEMATIC)) {
				rd->setKinematicTarget(physx::PxTransform(toPxVec3(getPosition()), toPxQuat(getRotation())));
			}
		}
	}
}