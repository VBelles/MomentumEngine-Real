#include "mcv_platform.h"
#include "module_scripting.h"
#include "modules/module.h"
#include <SLB/lua.hpp>
#include <SLB/SLB.hpp>
#include "modules/game/scripting/scripting_player.h"


CModuleScripting::CModuleScripting(const std::string& name) : IModule(name) {}


bool CModuleScripting::start() {
	manager = new SLB::Manager();
	script = new SLB::Script(manager);

	SLB::Class<ScriptingPlayer>("Player", manager)
		.constructor()
		.set("getHp", &ScriptingPlayer::getHp)
		.set("teleport", &ScriptingPlayer::teleport);

	script->setPrintCallback(&printCallback);
	script->doString("SLB.using(SLB)");
	script->doString("player = Player()");
	script->doString("print(\"Player hp: \" .. player:getHp())");
	
	return true;
}


bool CModuleScripting::stop() {
	SAFE_DELETE(manager);
	SAFE_DELETE(script);

	return true;
}

void CModuleScripting::update(float delta) {
}

void CModuleScripting::render() {
}

void printCallback(SLB::Script* script, const char* str, size_t strSize) {
	dbg(str);
}