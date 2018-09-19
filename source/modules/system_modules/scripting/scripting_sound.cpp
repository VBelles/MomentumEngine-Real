#include "mcv_platform.h"
#include "scripting_sound.h"
#include <SLB/SLB.hpp>

void ScriptingSound::bind(SLB::Manager* manager) {
	bindConstants(manager);
	manager->set("startSoundEvent", SLB::FuncCall::create(ScriptingSound::startSoundEvent));
	manager->set("stopSoundEvent", SLB::FuncCall::create(ScriptingSound::stopSoundEvent));
	manager->set("emitSoundEvent", SLB::FuncCall::create(ScriptingSound::emitSoundEvent));
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