#pragma once

#include "IBehaviorTreeNode.h"

class BehaviorTreeNodePriority : public IBehaviorTreeNode {
public:
	BehaviorTreeNodePriority(std::string name);

	void recalc(IBehaviorTree *behaviorTree) override;
};