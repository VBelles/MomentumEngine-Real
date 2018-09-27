#pragma once

#include "IBehaviorTreeNode.h"

class CBehaviorTreeNodePriority : public IBehaviorTreeNode {
public:
	CBehaviorTreeNodePriority(std::string name);

	void recalc(IBehaviorTree *behaviorTree, float delta = 0.f) override;

	virtual void debugInMenu();
	virtual void debugInMenu(IBehaviorTree* behaviorTree) override;
};
