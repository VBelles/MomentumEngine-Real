#pragma once

class IBehaviorTreeAction;

class BTActionFactory {
private:
	static BTActionFactory* instance;

	std::map<std::string, std::function<IBehaviorTreeAction*(void)>> factoryFunctionRegistry;

public:
	// Remove copy ctor
	BTActionFactory(const BTActionFactory&) = delete;

	BTActionFactory();
	~BTActionFactory();

	static BTActionFactory& get();

	void registerFactoryFunction(std::string name,
		std::function<IBehaviorTreeAction*(void)> classFactoryFunction);

	IBehaviorTreeAction* create(std::string name);
};

class BTActionRegisterer {
public:
	BTActionRegisterer(std::string className, std::function<IBehaviorTreeAction*(void)> classFactoryFunction) {
		BTActionFactory::get().registerFactoryFunction(className, classFactoryFunction);
	}
};

#define REGISTER_BTACTION(NAME, TYPE) \
	static BTActionRegisterer bTActionRegisterer(NAME, [](void)->IBehaviorTreeAction* { return new TYPE();});