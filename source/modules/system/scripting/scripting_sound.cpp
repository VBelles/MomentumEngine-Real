#include "mcv_platform.h"
#include "scripting_sound.h"
#include <SLB/SLB.hpp>

void ScriptingSound::bind(SLB::Manager* manager) {
	manager->set("startSoundEvent", SLB::FuncCall::create(ScriptingSound::startSoundEvent));
	manager->set("stopSoundEvent", SLB::FuncCall::create(ScriptingSound::stopSoundEvent));
	manager->set("emitSoundEvent", SLB::FuncCall::create(ScriptingSound::emitSoundEvent));
}

void ScriptingSound::startSoundEvent(std::string event) {
	EngineSound.startEvent(event);
}

void ScriptingSound::stopSoundEvent(std::string event, int mode) {
	EngineSound.stopEvent(event, static_cast<FMOD_STUDIO_STOP_MODE>(mode));
}

void ScriptingSound::emitSoundEvent(std::string event) {
	EngineSound.emitEvent(event);
}