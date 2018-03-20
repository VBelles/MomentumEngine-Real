#pragma once

#include <string>
#include <vector>

class IBehaviorTree;

#define RANDOM 0
#define SEQUENCE 1
#define PRIORITY 2
#define ACTION 3

#define STAY 0
#define LEAVE 1

class BehaviorTreeNode {
private:
	std::string name;
	int type;
	std::vector<BehaviorTreeNode *>children;
	BehaviorTreeNode *parent;
	BehaviorTreeNode *right;
public:
	BehaviorTreeNode(std::string name);
	bool isRoot();
	void setParent(BehaviorTreeNode *parent);
	void setRight(BehaviorTreeNode *right);
	void addChild(BehaviorTreeNode *child);
	void setType(int type);
	void recalc(IBehaviorTree *behaviorTree);
	std::string getName();

	void recalcRandom(IBehaviorTree *behaviorTree);
	void recalcSequence(IBehaviorTree *behaviorTree);
	void recalcPriority(IBehaviorTree *behaviorTree);
	void recalcAction(IBehaviorTree *behaviorTree);
};
