#include "mcv_platform.h"
#include "comp_activate_listener.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "components/comp_render.h"
#include "components/comp_collider.h"
#include "components/platforms/comp_platform_simple.h"
#include "components/controllers/comp_rigid_anim.h"
#include "components/controllers/comp_rigid_anims_director.h"
#include "entity/common_msgs.h"


DECL_OBJ_MANAGER("activate_listener", TCompActivateListener);

void TCompActivateListener::debugInMenu() {
}

void TCompActivateListener::registerMsgs() {
	DECL_MSG(TCompActivateListener, TMsgMechanismSystemActivated, onActivate);
	DECL_MSG(TCompActivateListener, TMsgMechanismSystemDeactivated, onDeactivate);
}

void TCompActivateListener::load(const json& j, TEntityParseContext& ctx) {
	renderEnabledOnActive = j.value("render_enabled_on_active", renderEnabledOnActive);
	colliderEnabledOnActive = j.value("collider_enabled_on_active", colliderEnabledOnActive);
	platformEnabledOnActive = j.value("platform_enabled_on_active", platformEnabledOnActive);
	animationEnabledOnActive = j.value("animation_enabled_on_active", animationEnabledOnActive);
}

void TCompActivateListener::onActivate(const TMsgMechanismSystemActivated & msg) {
	TCompRender* render = get<TCompRender>();
	if (render) {
		if (renderEnabledOnActive) {
			render->enable();
		}
		else {
			render->disable();
		}
	}
	TCompCollider* collider = get<TCompCollider>();
	if (collider) {
		if (colliderEnabledOnActive) {
			collider->create();
		}
		else {
			collider->destroy();
		}
	}
	TCompPlatformSimple* platform = get<TCompPlatformSimple>();
	if (platform){
		if (platformEnabledOnActive) {
			platform->setEnabled(true);
		}
		else {
			platform->setEnabled(false);
		}
	}
	TCompRigidAnim* anim = get<TCompRigidAnim>();
	if (anim) {
		if (animationEnabledOnActive) {
			anim->setIsMoving(true);
		}
		else {
			anim->setIsMoving(false);
		}
	}
	TCompRigidAnimsDirector* director = get<TCompRigidAnimsDirector>();
	if (director) {
		if (animationEnabledOnActive) {
			director->setIsMoving(true);
		}
		else {
			director->setIsMoving(false);
		}
	}
}

void TCompActivateListener::onDeactivate(const TMsgMechanismSystemDeactivated & msg) {
	TCompRender* render = get<TCompRender>();
	if (render) {
		if (renderEnabledOnActive) {
			render->disable();
		}
		else {
			render->enable();
		}
	}
	TCompCollider* collider = get<TCompCollider>();
	if (collider) {
		if (colliderEnabledOnActive) {
			collider->destroy();
		}
		else {
			collider->create();
		}
	}
	TCompPlatformSimple* platform = get<TCompPlatformSimple>();
	if (platform) {
		if (platformEnabledOnActive) {
			platform->setEnabled(false);
		}
		else {
			platform->setEnabled(true);
		}
	}
	TCompRigidAnim* anim = get<TCompRigidAnim>();
	if (anim) {
		if (animationEnabledOnActive) {
			anim->setIsMoving(false);
		}
		else {
			anim->setIsMoving(true);
		}
	}
	TCompRigidAnimsDirector* director = get<TCompRigidAnimsDirector>();
	if (director) {
		if (animationEnabledOnActive) {
			director->setIsMoving(false);
		}
		else {
			director->setIsMoving(true);
		}
	}
}
