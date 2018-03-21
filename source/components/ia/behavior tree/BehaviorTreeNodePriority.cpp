#include "mcv_platform.h"
#include "BehaviorTreeNodePriority.h"
#include "IBehaviorTree.h"

BehaviorTreeNodePriority::BehaviorTreeNodePriority(std::string name)
	: IBehaviorTreeNode::IBehaviorTreeNode(name) {
}

void BehaviorTreeNodePriority::recalc(IBehaviorTree *behaviorTree) {
	for (int i = 0; i < children.size(); i++) {
		if (behaviorTree->testCondition(children[i]->getName())) {
			children[i]->recalc(behaviorTree);
			break;
		}
	}
}