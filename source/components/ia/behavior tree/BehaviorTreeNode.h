#pragma once

#include <string>
#include <vector>

class IBehaviorTree;

enum EBehaviorTreeNodeType { Random, Sequence, Priority, Action };
enum EBehaviorTreeNodeEndAction { Stay, Leave };

class CBehaviorTreeNode {
private:
	std::string name;
	EBehaviorTreeNodeType type;
	std::vector<CBehaviorTreeNode *>children;
	CBehaviorTreeNode *parent;
	CBehaviorTreeNode *right;

public:
	CBehaviorTreeNode(std::string name);

	std::string getName();
	bool isRoot();

	void setType(EBehaviorTreeNodeType type);
	void setParent(CBehaviorTreeNode *parent);
	void setRight(CBehaviorTreeNode *right);
	void addChild(CBehaviorTreeNode *child);

	void recalc(IBehaviorTree *behaviorTree);
	void recalcRandom(IBehaviorTree *behaviorTree);
	void recalcSequence(IBehaviorTree *behaviorTree);
	void recalcPriority(IBehaviorTree *behaviorTree);
	void recalcAction(IBehaviorTree *behaviorTree);
};