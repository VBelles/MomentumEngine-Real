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
		if (!script->safeDoString(command)) {
			console->AddLog("[error]%s", script->getLastError());
		}
	});
	manager = new SLB::Manager();
	script = new SLB::Script(manager);
	script->setPrintCallback(&printCallback);

	SLB::Class<ScriptingPlayer>("Player", manager)
		.constructor()
		.set("getHp", &ScriptingPlayer::getHp)
		.set("teleport", &ScriptingPlayer::teleport)
		.set("tp", &ScriptingPlayer::teleport)
		.set("move", &ScriptingPlayer::move)
		.set("setPower", &ScriptingPlayer::setPower);


	script->doString("SLB.using(SLB)");
	script->doString("player = Player()");

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

void printCallback(SLB::Script* script, const char* str, size_t strSize) {
	dbg(str);
	Engine.getScripting().getConsole()->AddLog(str);
}