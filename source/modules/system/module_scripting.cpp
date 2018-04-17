#include "mcv_platform.h"
#include "module_scripting.h"
#include "modules/module.h"
#include <SLB/lua.hpp>
#include <SLB/SLB.hpp>
#include "modules/system/scripting/scripting_player.h"


CModuleScripting::CModuleScripting(const std::string & name) : IModule(name) {
}

bool CModuleScripting::start() {
	manager = new SLB::Manager();
	SLB::Class<ScriptingPlayer>("Player", manager)
		.constructor()
		.set("getHp", &ScriptingPlayer::getHp);
	SLB::Script s(manager);
	s.doString("SLB.using(SLB)");
	s.doString("player = Player()");
	s.doString("print(player:getHp())");
	return true;
}


bool CModuleScripting::stop() {
	SAFE_DELETE(manager);
	return true;
}

void CModuleScripting::update(float delta) {
}

void CModuleScripting::render() {
}
