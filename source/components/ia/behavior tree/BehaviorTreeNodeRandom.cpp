#include "mcv_platform.h"
#include "BehaviorTreeNodeRandom.h"
#include "IBehaviorTree.h"

CBehaviorTreeNodeRandom::CBehaviorTreeNodeRandom(std::string name)
	: IBehaviorTreeNode::IBehaviorTreeNode(name) {
}

void CBehaviorTreeNodeRandom::recalc(IBehaviorTree *behaviorTree) {
	int random = rand() % children.size();
	children[random]->recalc(behaviorTree);
}