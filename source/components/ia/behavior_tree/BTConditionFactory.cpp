#include "mcv_platform.h"
#include "BTConditionFactory.h"
#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

BTConditionFactory* BTConditionFactory::instance = nullptr;

BTConditionFactory::BTConditionFactory() {
	if (instance == nullptr) instance = this;
}

BTConditionFactory::~BTConditionFactory() {
	assert(instance == this);
	instance = nullptr;
}

BTConditionFactory& BTConditionFactory::get() {
	if (instance == nullptr) instance = new BTConditionFactory();
	return *instance;
}

void BTConditionFactory::registerFactoryFunction(std::string name,
	std::function<IBehaviorTreeCondition*(void)> classFactoryFunction) {

	factoryFunctionRegistry[name] = classFactoryFunction;
}

IBehaviorTreeCondition* BTConditionFactory::create(std::string name) {
	IBehaviorTreeCondition* instance = nullptr;

	// find name in the registry and call factory method.
	auto it = factoryFunctionRegistry.find(name);
	if (it != factoryFunctionRegistry.end())
		instance = it->second();

	return instance;
}
