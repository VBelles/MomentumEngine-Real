#pragma once

#include <string>
#include <map>
#include "BehaviorTreeNode.h"

typedef bool (IBehaviorTree::*BehaviorTreeCondition)();
typedef int (IBehaviorTree::*BehaviorTreeAction)();

class IBehaviorTree {
private:
	std::string name;

	std::map<std::string, BehaviorTreeNode *>tree;
	std::map<std::string, BehaviorTreeCondition> conditions;
	std::map<std::string, BehaviorTreeAction> actions;

	BehaviorTreeNode *root;
	BehaviorTreeNode *current;

	BehaviorTreeNode *createNode(std::string name);
	BehaviorTreeNode *findNode(std::string name);

public:
	IBehaviorTree(std::string name);

	std::string getName();

	BehaviorTreeNode *createRoot(std::string rootName, BehaviorTreeNodeType type, BehaviorTreeCondition condition, BehaviorTreeAction action);
	BehaviorTreeNode *addChild(std::string parentName, std::string childName, BehaviorTreeNodeType type, BehaviorTreeCondition condition, BehaviorTreeAction action);

	void setCurrent(BehaviorTreeNode *newCurrent);

	void addCondition(std::string conditionName, BehaviorTreeCondition condition);
	bool testCondition(std::string conditionName);

	void addAction(std::string actionName, BehaviorTreeAction action);
	int execAction(std::string actionName);

	void recalc();
};