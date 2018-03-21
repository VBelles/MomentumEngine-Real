#pragma once

#include <string>
#include <vector>

class IBehaviorTree;

enum EBehaviorTreeNodeType { Random, Sequence, Priority, Action };
enum EBehaviorTreeNodeEndAction { Stay, Leave };

class IBehaviorTreeNode {
protected:
public:
	std::string name;
	std::vector<IBehaviorTreeNode *>children;
	IBehaviorTreeNode *parent;
	IBehaviorTreeNode *right;

	IBehaviorTreeNode(std::string name);

	std::string getName();
	bool isRoot();

	void setParent(IBehaviorTreeNode *parent);
	void setRight(IBehaviorTreeNode *right);
	void addChild(IBehaviorTreeNode *child);

	virtual void recalc(IBehaviorTree *behaviorTree) {}
};