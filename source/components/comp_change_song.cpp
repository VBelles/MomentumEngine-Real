#include "mcv_platform.h"
#include "comp_change_song.h"

DECL_OBJ_MANAGER("change_song", TCompChangeSong);

void TCompChangeSong::debugInMenu() {
}

void TCompChangeSong::registerMsgs() {
	DECL_MSG(TCompChangeSong, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompChangeSong, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompChangeSong, TMsgTriggerExit, onTriggerExit);
	DECL_MSG(TCompChangeSong, TMsgColliderDestroyed, onColliderDestroyed);
}

void TCompChangeSong::load(const json & j, TEntityParseContext & ctx) {
	std::string song = j.value("song", "");//si se queda sin canción es que quieres parar la que hay
	fadeOutTime = j.value("fade_out_time", fadeOutTime);
}

void TCompChangeSong::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
	musicPlayerHandle = static_cast<CEntity*>(getEntityByName(MUSIC_PLAYER))->get<TCompMusic>();

	std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
	UniqueElement* uniqueEvent = EngineUniques.getUniqueEvent(name);
	if (uniqueEvent && uniqueEvent->done) {
		((TCompCollider*)get<TCompCollider>())->destroy();
	}
}

void TCompChangeSong::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity* entity = msg.h_other_entity;
	if (PLAYER_NAME == entity->getName()) {
		if (song == "") {
			getMusicPlayer()->fadeOut(fadeOutTime);
		}
		else {
			getMusicPlayer()->setCurrentSong(song);
		}
		std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
		UniqueElement* uniqueEvent = EngineUniques.getUniqueEvent(name);
		if (uniqueEvent && !uniqueEvent->done) {
			uniqueEvent->done = true;
			((TCompCollider*)get<TCompCollider>())->destroy();
		}
	}
}

void TCompChangeSong::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	//hacer esto al final de la secuencia
	CHandle(this).getOwner().destroy();
}

void TCompChangeSong::onTriggerExit(const TMsgTriggerExit & msg) {
}

TCompMusic* TCompChangeSong::getMusicPlayer() {
	return musicPlayerHandle;
}
