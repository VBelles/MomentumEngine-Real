#pragma once

class IBehaviorTreeNode;

class BTNodeFactory {
private:
	static BTNodeFactory* instance;

	std::map<std::string, std::function<IBehaviorTreeNode*(std::string)>> factoryFunctionRegistry;

public:
	// Remove copy ctor
	BTNodeFactory(const BTNodeFactory&) = delete;

	BTNodeFactory();
	~BTNodeFactory();

	static BTNodeFactory& get();

	void registerFactoryFunction(std::string name,
		std::function<IBehaviorTreeNode*(std::string name)> classFactoryFunction);

	IBehaviorTreeNode* create(std::string name, std::string nodeName);
};

class BTNodeRegisterer {
public:
	BTNodeRegisterer(std::string className, std::function<IBehaviorTreeNode*(std::string name)> classFactoryFunction) {
		BTNodeFactory::get().registerFactoryFunction(className, classFactoryFunction);
	}
};

#define REGISTER_BTNODE(NAME, TYPE) \
	static BTNodeRegisterer bTNodeRegisterer(NAME, [](std::string name)->IBehaviorTreeNode* { return new TYPE(name);});