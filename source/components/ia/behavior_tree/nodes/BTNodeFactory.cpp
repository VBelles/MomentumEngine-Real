#include "mcv_platform.h"
#include "BTNodeFactory.h"
#include "components/ia/behavior_tree/nodes/IBehaviorTreeNode.h"

BTNodeFactory* BTNodeFactory::instance = nullptr;

BTNodeFactory::BTNodeFactory() {
	if (instance == nullptr) instance = this;
}

BTNodeFactory::~BTNodeFactory() {
	assert(instance == this);
	instance = nullptr;
}

BTNodeFactory& BTNodeFactory::get() {
	if (instance == nullptr) instance = new BTNodeFactory();
	return *instance;
}

void BTNodeFactory::registerFactoryFunction(std::string name,
	std::function<IBehaviorTreeNode*(std::string name)> classFactoryFunction) {

	factoryFunctionRegistry[name] = classFactoryFunction;
}

IBehaviorTreeNode* BTNodeFactory::create(std::string name, std::string nodeName) {
	IBehaviorTreeNode* instance = nullptr;

	// find name in the registry and call factory method.
	auto it = factoryFunctionRegistry.find(name);
	if (it != factoryFunctionRegistry.end())
		instance = it->second(nodeName);

	return instance;
}
