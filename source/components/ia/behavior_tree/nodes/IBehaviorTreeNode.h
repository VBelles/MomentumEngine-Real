#pragma once

#include <string>
#include <vector>

#include "components/ia/behavior_tree/nodes/BTNodeFactory.h"

class IBehaviorTree;
class IBehaviorTreeNew;

enum EBehaviorTreeNodeType { Random, Sequence, Priority, Action };
enum EBehaviorTreeNodeEndAction { Stay, Leave };

class IBehaviorTreeNode {
protected:
	std::string name;
	std::vector<IBehaviorTreeNode*> children;
	IBehaviorTreeNode* parent;
	IBehaviorTreeNode* right;

public:
	IBehaviorTreeNode(std::string name);

	std::string getName();
	IBehaviorTreeNode* getParent();
	IBehaviorTreeNode* getRight();
	bool isRoot();

	void setParent(IBehaviorTreeNode* parent);
	void setRight(IBehaviorTreeNode* right);
	virtual void addChild(IBehaviorTreeNode* child);

	virtual void recalc(IBehaviorTree* behaviorTree, float delta = 0.f) {}
	virtual void recalc(IBehaviorTreeNew* behaviorTree, float delta = 0.f) {}

	virtual void debugInMenu();
	virtual void debugInMenu(IBehaviorTreeNew* behaviorTree);
	virtual void load(const json& j) {}
};