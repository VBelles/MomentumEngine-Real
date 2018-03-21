#pragma once

#include <string>
#include <map>
#include "IBehaviorTreeNode.h"

typedef bool (IBehaviorTree::*BehaviorTreeCondition)();
typedef int (IBehaviorTree::*BehaviorTreeAction)();

class IBehaviorTree {
private:
	std::string name;

	std::map<std::string, IBehaviorTreeNode *>tree;
	std::map<std::string, BehaviorTreeCondition> conditions;
	std::map<std::string, BehaviorTreeAction> actions;

	IBehaviorTreeNode *root;
	IBehaviorTreeNode *current;

	IBehaviorTreeNode *createNode(std::string name, EBehaviorTreeNodeType type);
	IBehaviorTreeNode *findNode(std::string name);

public:
	IBehaviorTree(std::string name);

	std::string getName();

	IBehaviorTreeNode *createRoot(std::string rootName, EBehaviorTreeNodeType type, BehaviorTreeCondition condition, BehaviorTreeAction action);
	IBehaviorTreeNode *addChild(std::string parentName, std::string childName, EBehaviorTreeNodeType type, BehaviorTreeCondition condition, BehaviorTreeAction action);

	void setCurrent(IBehaviorTreeNode *newCurrent);

	void addCondition(std::string conditionName, BehaviorTreeCondition condition);
	bool testCondition(std::string conditionName);

	void addAction(std::string actionName, BehaviorTreeAction action);
	int execAction(std::string actionName);

	void recalc();
};