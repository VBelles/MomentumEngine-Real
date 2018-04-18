#include "mcv_platform.h"
#include "module_scripting.h"
#include "modules/module.h"
#include <SLB/lua.hpp>
#include <SLB/SLB.hpp>
#include "modules/game/scripting/scripting_player.h"
#include "console.h"

CModuleScripting::CModuleScripting(const std::string& name) : IModule(name) {}


bool CModuleScripting::start() {
	console = new SimpleConsole([&](const char* command) {
		try {
			dbg("Executing command: %s\n", command);
			script->safeDoString(command);
		}
		catch (std::exception e) {
			console->AddLog("[error] Cant execute: ", command);
		}
	});
	manager = new SLB::Manager();
	script = new SLB::Script(manager);

	SLB::Class<ScriptingPlayer>("Player", manager)
		.constructor()
		.set("getHp", &ScriptingPlayer::getHp)
		.set("teleport", &ScriptingPlayer::teleport)
		.set("tp", &ScriptingPlayer::teleport)
		.set("move", &ScriptingPlayer::move)
		.set("setPower", &ScriptingPlayer::setPower);

	script->setPrintCallback(&printCallback);
	script->doString("SLB.using(SLB)");
	script->doString("player = Player()");
	
	//script->doString("player:move(0, 20, 0)");

	return true;
}


bool CModuleScripting::stop() {
	SAFE_DELETE(manager);
	SAFE_DELETE(script);
	SAFE_DELETE(console);
	return true;
}

void CModuleScripting::update(float delta) {
}

void CModuleScripting::render() {
	if (CApp::get().showDebug) {
		bool open = true;
		console->Draw("LUA Console", &open);
	}
}

void CModuleScripting::onPrint(SLB::Script* script, const char* str, size_t strSize) {
	dbg(str);
	
	console->AddLog(str);
}

void printCallback(SLB::Script* script, const char* str, size_t strSize) {
	Engine.getScripting().onPrint(script, str, strSize);
}