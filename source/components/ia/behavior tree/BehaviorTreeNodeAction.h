#pragma once

#include "IBehaviorTreeNode.h"

class BehaviorTreeNodeAction : public IBehaviorTreeNode {
public:
	BehaviorTreeNodeAction(std::string name);

	void recalc(IBehaviorTree *behaviorTree) override;
};