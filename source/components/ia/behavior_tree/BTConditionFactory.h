#pragma once

class IBehaviorTreeCondition;

class BTConditionFactory {
private:
	static BTConditionFactory* instance;

	std::map<std::string, std::function<IBehaviorTreeCondition*(void)>> factoryFunctionRegistry;

public:
	// Remove copy ctor
	BTConditionFactory(const BTConditionFactory&) = delete;

	BTConditionFactory();
	~BTConditionFactory();

	static BTConditionFactory& get();

	void registerFactoryFunction(std::string name,
		std::function<IBehaviorTreeCondition*(void)> classFactoryFunction);

	IBehaviorTreeCondition* create(std::string name);
};

class BTConditionRegisterer {
public:
	BTConditionRegisterer(std::string className, std::function<IBehaviorTreeCondition*(void)> classFactoryFunction) {
		BTConditionFactory::get().registerFactoryFunction(className, classFactoryFunction);
	}
};

#define REGISTER_BTCondition(NAME, TYPE) \
	static BTConditionRegisterer bTConditionRegisterer(NAME, [](void)->IBehaviorTreeCondition* { return new TYPE();});