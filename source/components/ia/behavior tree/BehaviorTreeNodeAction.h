#pragma once

#include "IBehaviorTreeNode.h"

class CBehaviorTreeNodeAction : public IBehaviorTreeNode {
public:
	CBehaviorTreeNodeAction(std::string name);

	void recalc(IBehaviorTree *behaviorTree) override;
};