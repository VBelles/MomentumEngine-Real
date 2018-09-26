#pragma once

#include "IBehaviorTreeNode.h"

class CBehaviorTreeNodeSequence : public IBehaviorTreeNode {
public:
	CBehaviorTreeNodeSequence(std::string name);

	void recalc(IBehaviorTree *behaviorTree, float delta = 0.f) override;

	virtual void debugInMenu();
	virtual void debugInMenu(IBehaviorTree* behaviorTree) override;
};
