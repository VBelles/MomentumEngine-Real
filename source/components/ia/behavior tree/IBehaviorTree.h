#pragma once

#include <string>
#include <map>
#include "IBehaviorTreeNode.h"

typedef bool (IBehaviorTree::*BehaviorTreeCondition)(float delta);
typedef int (IBehaviorTree::*BehaviorTreeAction)(float delta);

class IBehaviorTree {
protected:
	std::map<std::string, IBehaviorTreeNode *>tree;
	std::map<std::string, BehaviorTreeCondition> conditions;
	std::map<std::string, BehaviorTreeAction> actions;

	IBehaviorTreeNode *root;
	IBehaviorTreeNode *current;

	IBehaviorTreeNode *createNode(std::string name, EBehaviorTreeNodeType type);
	IBehaviorTreeNode *findNode(std::string name);

	bool falseCondition(float delta = 0.f);
	bool trueCondition(float delta = 0.f);

public:
	IBehaviorTree();

	IBehaviorTreeNode *createRoot(std::string rootName, EBehaviorTreeNodeType type, BehaviorTreeCondition condition, BehaviorTreeAction action);
	IBehaviorTreeNode *addChild(std::string parentName, std::string childName, EBehaviorTreeNodeType type, BehaviorTreeCondition condition, BehaviorTreeAction action);

	void setCurrent(IBehaviorTreeNode *newCurrent);

	void addCondition(std::string conditionName, BehaviorTreeCondition condition);
	bool testCondition(std::string conditionName, float delta = 0.f);

	void addAction(std::string actionName, BehaviorTreeAction action);
	int execAction(std::string actionName, float delta = 0.f);

	void recalc(float delta = 0.f);
};