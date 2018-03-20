#pragma once

#include <string>
#include <vector>

class IBehaviorTree;

enum BehaviorTreeNodeType { Random, Sequence, Priority, Action };
enum BehaviorTreeNodeEndAction { Stay, Leave };

class BehaviorTreeNode {
private:
	std::string name;
	BehaviorTreeNodeType type;
	std::vector<BehaviorTreeNode *>children;
	BehaviorTreeNode *parent;
	BehaviorTreeNode *right;

public:
	BehaviorTreeNode(std::string name);

	std::string getName();
	bool isRoot();

	void setType(BehaviorTreeNodeType type);
	void setParent(BehaviorTreeNode *parent);
	void setRight(BehaviorTreeNode *right);
	void addChild(BehaviorTreeNode *child);

	void recalc(IBehaviorTree *behaviorTree);
	void recalcRandom(IBehaviorTree *behaviorTree);
	void recalcSequence(IBehaviorTree *behaviorTree);
	void recalcPriority(IBehaviorTree *behaviorTree);
	void recalcAction(IBehaviorTree *behaviorTree);
};