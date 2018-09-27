#include "mcv_platform.h"
#include "comp_power_up.h"
#include "components/player/comp_player_model.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("power_up", TCompPowerUp);

void TCompPowerUp::debugInMenu() {
	ImGui::Text("I give you: %s", stateToUnlock.c_str());
}

void TCompPowerUp::registerMsgs() {
	DECL_MSG(TCompPowerUp, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompPowerUp, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompPowerUp, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompPowerUp, TMsgColliderDestroyed, onColliderDestroyed);
}

void TCompPowerUp::load(const json& j, TEntityParseContext& ctx) {
	stateToUnlock = j.value("state_to_unlock","");
}

void TCompPowerUp::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	transformHandle = get<TCompTransform>();
	assert(transformHandle.isValid());
}

void TCompPowerUp::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
	CEntity* player = getEntityByName(PLAYER_NAME);
	playerModelHandle = player->get<TCompPlayerModel>();
	assert(playerModelHandle.isValid());
}

void TCompPowerUp::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity* entity = msg.h_other_entity;
	if (entity->getName() == PLAYER_NAME) {
		//unlock state in player manager through player model
		((TCompPlayerModel*)getPlayerModel())->unlockState(stateToUnlock);

		std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
		UniquePowerUp* uniquePowerUp = EngineUniques.getUniquePowerUp(name);
		if (uniquePowerUp && !uniquePowerUp->done) {
			uniquePowerUp->done = true;
		}
		((TCompCollider*)get<TCompCollider>())->destroy();
	}
}

void TCompPowerUp::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	CHandle(this).getOwner().destroy();
}

TCompTransform* TCompPowerUp::getTransform() {
	return transformHandle;
}

TCompPlayerModel* TCompPowerUp::getPlayerModel() {
	return playerModelHandle;
}
