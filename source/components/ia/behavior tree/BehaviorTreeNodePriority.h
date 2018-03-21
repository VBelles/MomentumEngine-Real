#pragma once

#include "IBehaviorTreeNode.h"

class CBehaviorTreeNodePriority : public IBehaviorTreeNode {
public:
	CBehaviorTreeNodePriority(std::string name);

	void recalc(IBehaviorTree *behaviorTree) override;
};