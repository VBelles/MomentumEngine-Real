#include "mcv_platform.h"
#include "module_slash.h"


CModuleSlash::CModuleSlash(const std::string& name) 
	: IModule(name) {
}

bool CModuleSlash::start() {
	return true;
}

bool CModuleSlash::stop() {
	return true;
}

void CModuleSlash::update(float delta) {
}

void CModuleSlash::render() {
}


