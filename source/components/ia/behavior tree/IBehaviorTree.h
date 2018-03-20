#pragma once

#include <string>
#include <map>
#include "BehaviorTreeNode.h"

typedef bool (IBehaviorTree::*BehaviorTreeCondition)();
typedef int (IBehaviorTree::*BehaviorTreeAction)();

class IBehaviorTree {
private:
	std::string name;

	std::map<std::string, CBehaviorTreeNode *>tree;
	std::map<std::string, BehaviorTreeCondition> conditions;
	std::map<std::string, BehaviorTreeAction> actions;

	CBehaviorTreeNode *root;
	CBehaviorTreeNode *current;

	CBehaviorTreeNode *createNode(std::string name);
	CBehaviorTreeNode *findNode(std::string name);

public:
	IBehaviorTree(std::string name);

	std::string getName();

	CBehaviorTreeNode *createRoot(std::string rootName, EBehaviorTreeNodeType type, BehaviorTreeCondition condition, BehaviorTreeAction action);
	CBehaviorTreeNode *addChild(std::string parentName, std::string childName, EBehaviorTreeNodeType type, BehaviorTreeCondition condition, BehaviorTreeAction action);

	void setCurrent(CBehaviorTreeNode *newCurrent);

	void addCondition(std::string conditionName, BehaviorTreeCondition condition);
	bool testCondition(std::string conditionName);

	void addAction(std::string actionName, BehaviorTreeAction action);
	int execAction(std::string actionName);

	void recalc();
};