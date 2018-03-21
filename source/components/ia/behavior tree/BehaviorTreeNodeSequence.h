#pragma once

#include "IBehaviorTreeNode.h"

class BehaviorTreeNodeSequence : public IBehaviorTreeNode {
public:
	BehaviorTreeNodeSequence(std::string name);

	void recalc(IBehaviorTree *behaviorTree) override;
};