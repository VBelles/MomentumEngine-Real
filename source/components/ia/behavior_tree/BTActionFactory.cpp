#include "mcv_platform.h"
#include "BTActionFactory.h"
#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

BTActionFactory* BTActionFactory::instance = nullptr;

BTActionFactory::BTActionFactory() {
	if (instance == nullptr) instance = this;
}

BTActionFactory::~BTActionFactory() {
	assert(instance == this);
	instance = nullptr;
}

BTActionFactory& BTActionFactory::get() {
	if (instance == nullptr) instance = new BTActionFactory();
	return *instance;
}

void BTActionFactory::registerFactoryFunction(std::string name,
	std::function<IBehaviorTreeAction*(void)> classFactoryFunction) {

	factoryFunctionRegistry[name] = classFactoryFunction;
}

IBehaviorTreeAction* BTActionFactory::create(std::string name) {
	IBehaviorTreeAction* instance = nullptr;

	// find name in the registry and call factory method.
	auto it = factoryFunctionRegistry.find(name);
	if (it != factoryFunctionRegistry.end()) {
		instance = it->second();
	}

	return instance;
}