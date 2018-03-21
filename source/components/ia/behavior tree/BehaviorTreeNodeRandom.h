#pragma once

#include "IBehaviorTreeNode.h"

class BehaviorTreeNodeRandom : public IBehaviorTreeNode {
public:
	BehaviorTreeNodeRandom(std::string name);

	void recalc(IBehaviorTree *behaviorTree) override;
};