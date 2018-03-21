#pragma once

#include "IBehaviorTreeNode.h"

class CBehaviorTreeNodeSequence : public IBehaviorTreeNode {
public:
	CBehaviorTreeNodeSequence(std::string name);

	void recalc(IBehaviorTree *behaviorTree) override;
};