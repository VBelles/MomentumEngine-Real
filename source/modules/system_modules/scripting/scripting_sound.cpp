#include "mcv_platform.h"
#include "scripting_sound.h"
#include <SLB/SLB.hpp>
#include "modules/system_modules/sound/music_player.h"

void ScriptingSound::bind(SLB::Manager* manager) {
	bindConstants(manager);
	manager->set("startSoundEvent", SLB::FuncCall::create(ScriptingSound::startSoundEvent));
	manager->set("stopSoundEvent", SLB::FuncCall::create(ScriptingSound::stopSoundEvent));
	manager->set("emitSoundEvent", SLB::FuncCall::create(ScriptingSound::emitSoundEvent));
	manager->set("stopMusic", SLB::FuncCall::create(ScriptingSound::stopMusic));
	manager->set("startSong", SLB::FuncCall::create(ScriptingSound::startSong));
}

void ScriptingSound::bindConstants(SLB::Manager* manager) {
	manager->set("SOUND_STOP_ALLOWFADEOUT", SLB::Value::copy(static_cast<int>(FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT)));
	manager->set("SOUND_STOP_IMMEDIATE", SLB::Value::copy(static_cast<int>(FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_IMMEDIATE)));
}

void ScriptingSound::startSoundEvent(std::string event) {
	//EngineSound.startEvent(event.c_str());
}

void ScriptingSound::stopSoundEvent(std::string event, int mode) {
	//EngineSound.stopEvent(event.c_str(), static_cast<FMOD_STUDIO_STOP_MODE>(mode));
}

void ScriptingSound::emitSoundEvent(std::string event) {
	EngineSound.emitEvent(event.c_str());
}

void ScriptingSound::stopMusic(float time) {
	//debería hacerse con un fadeout
	getMusicPlayer()->fadeOut(time);
}

void ScriptingSound::startSong(std::string song) {
	//intro, main
	getMusicPlayer()->setCurrentSong(song);
}

CMusicPlayer * ScriptingSound::getMusicPlayer() {
	return EngineSound.getMusicPlayer();
}
