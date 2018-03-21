#pragma once

#include "IBehaviorTreeNode.h"

class CBehaviorTreeNodeRandom : public IBehaviorTreeNode {
public:
	CBehaviorTreeNodeRandom(std::string name);

	void recalc(IBehaviorTree *behaviorTree) override;
};