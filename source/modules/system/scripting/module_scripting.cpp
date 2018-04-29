#include "mcv_platform.h"
#include "module_scripting.h"
#include "modules/module.h"
#include <SLB/lua.hpp>
#include <SLB/SLB.hpp>
#include "console.h"
#include "modules/system/scripting/scripting_player.h"
#include "modules/system/scripting/scripting_golem.h"
#include "modules/system/scripting/scripting_entities.h"
#include "modules/system/scripting/scripting_door.h"

CModuleScripting::CModuleScripting(const std::string& name) : IModule(name) {}

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

	//Game constants
	script->set("PLAYER_NAME", PLAYER_NAME);
	script->set("PLAYER_CAMERA", PLAYER_CAMERA);
	script->set("GAME_CAMERA", GAME_CAMERA);

	//Bind clases
	ScriptingPlayer::bind(manager);
	ScriptingGolem::bind(manager);
	ScriptingDoor::bind(manager);
	ScriptingEntities::create();
	ScriptingEntities::bind(manager);

	//Create binded objects
	execString("SLB.using(SLB)");
	execString("player = Player()");
	execString("golem = Golem()");

	execFile("data/test.lua");
}

bool CModuleScripting::start() {
	luaCalls = {
		{ onGameStart, "onGameStart" },
		{ onLevelStart, "onLevelStart" },
		{ onPowerLevelChange, "onPowerLevelChange" },
		{ onTriggerEnter, "onTriggerEnter" },
		{ onTriggerExit, "onTriggerExit" },
		{ onItemDestroyed, "onItemDestroyed" },
		{ onEnemyKilled, "onEnemyKilled" },
		{ onItemCollected, "onItemCollected" },
		{ onPlayerKilled, "onPlayerKilled" }
	};

	initConsole();
	initSLB();
	timer.reset();
	return true;
}

bool CModuleScripting::stop() {
	SAFE_DELETE(manager);
	SAFE_DELETE(script);
	SAFE_DELETE(console);
	ScriptingEntities::destroy();
	return true;
}

void CModuleScripting::update(float delta) {
	float currentTime = timer.elapsed();
}

void CModuleScripting::render() {
	//Render console on debug
	if (CApp::get().showDebug) {
		bool open = true;
		console->Draw("LUA Console", &open);
	}
}

void CModuleScripting::doFile(const char* filename) {
	execFile(filename);
}

void CModuleScripting::doFile(std::string filename) {
	execFile(filename.c_str());
}

void CModuleScripting::throwEvent(luaCall event, std::string params) {
	execString((luaCalls[event] + "(" + params + ")").c_str());
}

void CModuleScripting::execString(const char* string) {
	try {
		script->safeDoString(string);
	}
	catch (...) {}
}

void CModuleScripting::execFile(const char* string) {
	try {
		script->safeDoFile(string);
	}
	catch (...) {}
}

void printCallback(SLB::Script* script, const char* str, size_t strSize) {
	dbg(str);
	Engine.getScripting().getConsole()->AddLog(str);
}