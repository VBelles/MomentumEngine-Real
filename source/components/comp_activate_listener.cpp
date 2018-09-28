#include "mcv_platform.h"
#include "comp_activate_listener.h"
#include "entity/entity_parser.h"
#include "components/platforms/comp_platform_simple.h"
#include "components/platforms/comp_platforms_director.h"
#include "components/controllers/comp_rigid_anim.h"
#include "components/controllers/comp_rigid_anims_director.h"
#include "entity/common_msgs.h"


DECL_OBJ_MANAGER("activate_listener", TCompActivateListener);

void TCompActivateListener::debugInMenu() {
}

void TCompActivateListener::registerMsgs() {
	DECL_MSG(TCompActivateListener, TMsgEntityCreated, onEntityCreated);
	DECL_MSG(TCompActivateListener, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompActivateListener, TMsgMechanismSystemActivated, onActivate);
	DECL_MSG(TCompActivateListener, TMsgMechanismSystemDeactivated, onDeactivate);
}

void TCompActivateListener::load(const json& j, TEntityParseContext& ctx) {
	renderEnabledOnActive = j.value("render_enabled_on_active", renderEnabledOnActive);
	colliderEnabledOnActive = j.value("collider_enabled_on_active", colliderEnabledOnActive);
	platformEnabledOnActive = j.value("platform_enabled_on_active", platformEnabledOnActive);
	animationEnabledOnActive = j.value("animation_enabled_on_active", animationEnabledOnActive);

	changeRender = j.value("change_render", changeRender);
	changeCollider = j.value("change_collider", changeCollider);
	changePlatform = j.value("change_platform", changePlatform);
	changeAnim = j.value("change_animation", changeAnim);
}

void TCompActivateListener::onEntityCreated(const TMsgEntityCreated & msg) {
}

void TCompActivateListener::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
	if (changeRender) {
		TCompRender* render = get<TCompRender>();
		if (render) {
			if (renderEnabledOnActive) {
				render->disable();
			}
			else {
				render->enable();
			}
		}
	}
	if (changeCollider) {
		TCompCollider* collider = get<TCompCollider>();
		if (collider) {
			if (colliderEnabledOnActive) {
				collider->destroy();
			}
			else {
				collider->create();
			}
		}
	}

	if (changePlatform) {
		TCompPlatformsDirector* platformsDirector = get<TCompPlatformsDirector>();
		TCompPlatformSimple* platform = get<TCompPlatformSimple>();
		if (platformsDirector) {
			platformsDirector->setEnabled(!platformEnabledOnActive);
		}
		else if (platform) {
			platform->setEnabled(!platformEnabledOnActive);
		}
	}

	if (changeAnim) {
		TCompRigidAnimsDirector* animsDirector = get<TCompRigidAnimsDirector>();
		TCompRigidAnim* anim = get<TCompRigidAnim>();
		if (animsDirector) {
			animsDirector->setIsMoving(!animationEnabledOnActive);
		}
		else if (anim) {
			anim->setIsMoving(!animationEnabledOnActive);
		}
	}
}

void TCompActivateListener::onActivate(const TMsgMechanismSystemActivated & msg) {
	if (changeRender) {
		TCompRender* render = get<TCompRender>();
		if (render) {
			if (renderEnabledOnActive) {
				render->enable();
			}
			else {
				render->disable();
			}
		}
	}
	if (changeCollider) {
		TCompCollider* collider = get<TCompCollider>();
		if (collider) {
			if (colliderEnabledOnActive) {
				collider->create();
			}
			else {
				collider->destroy();
			}
		}
	}
	if (changePlatform) {
		TCompPlatformsDirector* platformsDirector = get<TCompPlatformsDirector>();
		if (platformsDirector) {
			if (platformEnabledOnActive) {
				platformsDirector->setEnabled(true);
			}
			else {
				platformsDirector->setEnabled(false);
			}
		}
		else {
			TCompPlatformSimple* platform = get<TCompPlatformSimple>();
			if (platform) {
				if (platformEnabledOnActive) {
					platform->setEnabled(true);
				}
				else {
					platform->setEnabled(false);
				}
			}
		}
	}
	if (changeAnim) {
		TCompRigidAnimsDirector* animsDirector = get<TCompRigidAnimsDirector>();
		if (animsDirector) {
			if (animationEnabledOnActive) {
				animsDirector->setIsMoving(true);
			}
			else {
				animsDirector->setIsMoving(false);
			}
		}
		else {
			TCompRigidAnim* anim = get<TCompRigidAnim>();
			if (anim) {
				if (animationEnabledOnActive) {
					anim->setIsMoving(true);
				}
				else {
					anim->setIsMoving(false);
				}
			}
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
	TCompPlatformsDirector* platformsDirector = get<TCompPlatformsDirector>();
	if (platformsDirector) {
		if (platformEnabledOnActive) {
			platformsDirector->setEnabled(false);
		}
		else {
			platformsDirector->setEnabled(true);
		}
	}
	else {
		TCompPlatformSimple* platform = get<TCompPlatformSimple>();
		if (platform) {
			if (platformEnabledOnActive) {
				platform->setEnabled(false);
			}
			else {
				platform->setEnabled(true);
			}
		}
	}
	TCompRigidAnimsDirector* animsDirector = get<TCompRigidAnimsDirector>();
	if (animsDirector) {
		if (animationEnabledOnActive) {
			animsDirector->setIsMoving(false);
		}
		else {
			animsDirector->setIsMoving(true);
		}
	}
	else {
		TCompRigidAnim* anim = get<TCompRigidAnim>();
		if (anim) {
			if (animationEnabledOnActive) {
				anim->setIsMoving(false);
			}
			else {
				anim->setIsMoving(true);
			}
		}
	}
}
