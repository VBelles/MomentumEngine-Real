#include "mcv_platform.h"
#include "BehaviorTreeNodeRandom.h"
#include "IBehaviorTree.h"

BehaviorTreeNodeRandom::BehaviorTreeNodeRandom(std::string name)
	: IBehaviorTreeNode::IBehaviorTreeNode(name) {
}

void BehaviorTreeNodeRandom::recalc(IBehaviorTree *behaviorTree) {
	int random = rand() % children.size();
	children[random]->recalc(behaviorTree);
}