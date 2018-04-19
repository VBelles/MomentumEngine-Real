#include "mcv_platform.h"
#include "module_scripting.h"
#include "modules/module.h"
#include <SLB/lua.hpp>
#include <SLB/SLB.hpp>
#include "console.h"
#include "modules/game/scripting/scripting_player.h"
#include "modules/game/scripting/scripting_golem.h"
#include "modules/game/scripting/scripting_entities.h"
#include "modules/game/scripting/scripting_door.h"

CModuleScripting::CModuleScripting(const std::string& name) : IModule(name) {}

void hello() {
	dbg("Hello\n");
}

bool CModuleScripting::start() {

	//Create console and console callback
	console = new SimpleConsole([&](const char* command) {
		if (_stricmp(command, "HELP") == 0) {
			console->AddLog("- Bindings:");
			script->safeDoString("print(player)");
			script->safeDoString("print(golem)");
		}
		else {
			std::string comandStr = command;
			comandStr = "print(" + comandStr + ")";
			dbg("%s\n", comandStr.c_str());
			if (!script->safeDoString(comandStr.c_str())) {
				console->AddLog("[error]%s", script->getLastError());
				dbg("%s\n", script->getLastError());
			}
		}
	});

	//Init SLB
	manager = new SLB::Manager();
	script = new SLB::Script(manager);
	script->setPrintCallback(&printCallback);

	//Bind clases
	ScriptingPlayer::bind(manager);
	ScriptingGolem::bind(manager);
	ScriptingDoor::bind(manager);
	ScriptingEntities::create();
	ScriptingEntities::bind(manager);

	//Create binded objects
	script->doString("SLB.using(SLB)");
	script->doString("player = Player()");
	script->doString("golem = Golem()");

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
}

void CModuleScripting::render() {
	//Render console on debug
	if (CApp::get().showDebug) {
		bool open = true;
		console->Draw("LUA Console", &open);
	}
}

void CModuleScripting::doFile(const char* filename) {
	script->doFile(filename);
}

void CModuleScripting::doFile(std::string filename) {
	if (!script->safeDoFile(filename.c_str())) {
		dbg("%s\n", script->getLastError());
	}
}

void printCallback(SLB::Script* script, const char* str, size_t strSize) {
	dbg(str);
	Engine.getScripting().getConsole()->AddLog(str);
}