#include "mcv_platform.h"
#include "BehaviorTreeNodeSequence.h"
#include "IBehaviorTree.h"

CBehaviorTreeNodeSequence::CBehaviorTreeNodeSequence(std::string name)
	: IBehaviorTreeNode::IBehaviorTreeNode(name) {
}

void CBehaviorTreeNodeSequence::recalc(IBehaviorTree *behaviorTree) {
	children[0]->recalc(behaviorTree);
}