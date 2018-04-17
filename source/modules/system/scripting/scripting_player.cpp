#include "mcv_platform.h"
#include "scripting_player.h"
#include "components/player/comp_player_model.h"


ScriptingPlayer::ScriptingPlayer() {
	//CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	//playerModelHandle = playerEntity->get<TCompPlayerModel>();
}

ScriptingPlayer::~ScriptingPlayer() {
}

float ScriptingPlayer::getHp() {
	return 8.f;
	//return getPlayerModel()->getHp();
}

TCompPlayerModel* ScriptingPlayer::getPlayerModel() {
	return playerModelHandle;
}


