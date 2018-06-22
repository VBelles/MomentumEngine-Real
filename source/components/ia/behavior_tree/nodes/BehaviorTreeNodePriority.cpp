#include "mcv_platform.h"
#include "BehaviorTreeNodePriority.h"
#include "components/ia/behavior_tree/IBehaviorTree.h"

CBehaviorTreeNodePriority::CBehaviorTreeNodePriority(std::string name)
	: IBehaviorTreeNode::IBehaviorTreeNode(name) {
}

void CBehaviorTreeNodePriority::recalc(IBehaviorTree *behaviorTree, float delta) {
	for (int i = 0; i < children.size(); i++) {
		if (behaviorTree->testCondition(children[i]->getName(), delta)) {
			children[i]->recalc(behaviorTree, delta);
			break;
		}
	}
}