#include "mcv_platform.h"
#include "module_scripting.h"
#include "modules/module.h"
#include <SLB/lua.hpp>

CModuleScripting::CModuleScripting(const std::string & name) : IModule(name) {
}

bool CModuleScripting::start() {
	state = luaL_newstate();
	return true;
}


bool CModuleScripting::stop() {
	lua_close(state);
	return true;
}

void CModuleScripting::update(float delta) {
}

void CModuleScripting::render() {
}
