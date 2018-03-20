#pragma once

#include <string>
#include <map>
#include "BehaviorTreeNode.h"

typedef bool (IBehaviorTree::*BehaviorTreeCondition)();
typedef int (IBehaviorTree::*BehaviorTreeAction)();

class IBehaviorTree {
private:
	std::map<std::string, BehaviorTreeNode *>tree;
	std::map<std::string, BehaviorTreeCondition> conditions;
	std::map<std::string, BehaviorTreeAction> actions;

	BehaviorTreeNode *root;
	BehaviorTreeNode *current;

	BehaviorTreeNode *createNode(std::string);
	BehaviorTreeNode *findNode(std::string);
public:
	std::string name;
	virtual void create(std::string);
	BehaviorTreeNode *createRoot(std::string, int, BehaviorTreeCondition, BehaviorTreeAction);
	BehaviorTreeNode *addChild(std::string, std::string, int, BehaviorTreeCondition, BehaviorTreeAction);

	void addCondition(std::string, BehaviorTreeCondition);
	bool testCondition(std::string);
	void addAction(std::string, BehaviorTreeAction);
	int execAction(std::string);
	void setCurrent(BehaviorTreeNode *);

	void recalc();
};
