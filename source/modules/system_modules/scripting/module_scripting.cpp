#include "mcv_platform.h"
#include "module_scripting.h"
#include "modules/module.h"
#include <SLB/lua.hpp>
#include <SLB/SLB.hpp>
#include "console.h"
#include "modules/system_modules/scripting/scripting_player.h"
#include "modules/system_modules/scripting/scripting_entities.h"
#include "modules/system_modules/scripting/scripting_door.h"
#include "modules/system_modules/scripting/scripting_manager.h"
#include "modules/system_modules/scripting/scripting_cameras.h"
#include "modules/system_modules/scripting/scripting_sound.h"
#include "modules/system_modules/scripting/scripting_particles.h"
#include "modules/system_modules/scripting/scripting_gui.h"

CModuleScripting* CModuleScripting::instance = nullptr;

CModuleScripting::CModuleScripting(const std::string& name) : IModule(name) {
	instance = this;
}

void CModuleScripting::initConsole() {
	//Create console and console callback
	console = new SimpleConsole([&](const char* command) {
		if (_stricmp(command, "HELP") == 0) {
			console->AddLog("- Bindings:");
			execString("print(player)");
			execString("print(golem)");
		}
		else {
			std::string comandStr = command;
			comandStr = "print(" + comandStr + ")";
			dbg("%s\n", comandStr.c_str());
			execString(comandStr.c_str());
		}
	});
}

void CModuleScripting::initSLB() {
	//Init SLB
	manager = new SLB::Manager();
	script = new SLB::Script(manager);
	script->setPrintCallback(&printCallback);

	//Bind clases
	ScriptingManager::bind(manager);
	ScriptingPlayer::bind(manager);
	ScriptingCameras::bind(manager);
	ScriptingEntities::create();
	ScriptingEntities::bind(manager);
	ScriptingDoor::bind(manager);
	ScriptingSound::bind(manager);
	ScriptingParticles::bind(manager);
	ScriptingGUI::bind(manager);

	//Create binded objects
	execString("SLB.using(SLB)");

	//Load scripts
	execFile("data/scripts/coroutines.lua");

	std::set<std::string> filesToLoad = WindowsUtils::getAllFiles(AUTOLOAD_SCRIPTS_PATH, "*.lua", true);
	for (std::string file : filesToLoad) {
		execFile(file.c_str());
	}
}

bool CModuleScripting::start() {
	luaCalls = {
		{ onGameStart, "onGameStart" },
		{ onLevelStart, "onLevelStart" },
		{ onPowerLevelChange, "onPowerLevelChange" },
		{ onTriggerEnter, "onTriggerEnter" },
		{ onTriggerExit, "onTriggerExit" },
		{ onEnemyKilled, "onEnemyKilled" },
		{ onPlayerKilled, "onPlayerKilled" },
		{ onAltarDestroyed, "onAltarDestroyed" },
		{ onMechanismSystemActivated, "onMechanismSystemActivated" },
		{ onMechanismSystemDeactivated, "onMechanismSystemDeactivated" },
		{ lastChrysalisTaken, "lastChrysalisTaken" },
		{ firstChrysalisTaken, "firstChrysalisTaken" },
		{ finalDoorCutscene, "finalDoorCutscene"}
	};

	callsFirstParamOnFunction = {
		onLevelStart,
		onTriggerEnter,
		onTriggerExit,
		onAltarDestroyed,
		onMechanismSystemActivated,
		onMechanismSystemDeactivated
	};

	initConsole();
	initSLB();

	nextCoroutineId = 0;
	delayedCalls.clear();

	timer.reset();
	return true;
}

bool CModuleScripting::stop() {
	safeDelete(manager);
	safeDelete(script);
	safeDelete(console);
	ScriptingEntities::destroy();
	return true;
}

void CModuleScripting::reset() {
	delayedCalls.clear();
	std::string call = "__clearCoroutines()";
	execString(call.c_str());
}

void CModuleScripting::update(float delta) {
	checkDelayedCalls();
	std::string call = "__updateCoroutines(" + std::to_string(delta) + ")";
	execString(call.c_str());
}

void CModuleScripting::render() {
	//Render console on debug
	if (CApp::get().isDebug()) {
		bool open = true;
		console->Draw("LUA Console", &open);
	}
}

void CModuleScripting::checkDelayedCalls() {
	std::set<DelayedCall> toRemove;
	float currentTime = timer.elapsed();
	for (DelayedCall dc : delayedCalls) {
		if (currentTime >= (dc.startTime + dc.delay)) {
			std::string call = "__startCoroutine(\"" + getNextCoroutineId() + "\"," + dc.func + "," + dc.params + ")";
			dbg("%s \n", call.c_str());
			execString(call.c_str());
			toRemove.insert(dc);
		}
	}

	for (DelayedCall dc : toRemove) {
		delayedCalls.erase(dc);
	}
}

void CModuleScripting::execString(const char* string) {
	script->safeDoString(string);
}

void CModuleScripting::execFile(const char* string) {
	script->safeDoFile(string);
}

std::string CModuleScripting::getNextCoroutineId() {
	std::string coroutineId = std::to_string(nextCoroutineId);
	nextCoroutineId++;
	return coroutineId;
}

void CModuleScripting::doFile(const char* filename) {
	execFile(filename);
}

void CModuleScripting::doFile(std::string filename) {
	execFile(filename.c_str());
}

void CModuleScripting::throwEvent(LuaCall event, std::string params) {
	std::string call;
	if (callsFirstParamOnFunction.find(event) != callsFirstParamOnFunction.end()) {
		size_t delimiterPos = params.find(",");
		std::string firstParam = params.substr(0, delimiterPos);
		std::string otherParams = "";
		if (delimiterPos != std::string::npos) {
			otherParams = ",\"" + params.substr(delimiterPos + 1) + "\"";
		}
		call = "__startCoroutine(\"" + getNextCoroutineId() + "\"," + luaCalls[event] + "_" + firstParam + otherParams + ")";
	}
	else {
		call = "__startCoroutine(\"" + getNextCoroutineId() + "\"," + luaCalls[event] + ",\"" + params + "\")";
	}
	execString(call.c_str());
}

void CModuleScripting::callDelayed(float delay, const char* func, const char* params) {
	delayedCalls.insert(DelayedCall{ timer.elapsed(), delay, func, params });
}

void printCallback(SLB::Script* script, const char* str, size_t strSize) {
	dbg(str);
	EngineScripting.getConsole()->AddLog(str);
}
