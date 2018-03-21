#include "mcv_platform.h"
#include "BehaviorTreeNodeSequence.h"
#include "IBehaviorTree.h"

BehaviorTreeNodeSequence::BehaviorTreeNodeSequence(std::string name)
	: IBehaviorTreeNode::IBehaviorTreeNode(name) {
}

void BehaviorTreeNodeSequence::recalc(IBehaviorTree *behaviorTree) {
	children[0]->recalc(behaviorTree);
}