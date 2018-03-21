#include "mcv_platform.h"
#include "BehaviorTreeNodeAction.h"
#include "BehaviorTreeNodeSequence.h"
#include "IBehaviorTree.h"

CBehaviorTreeNodeAction::CBehaviorTreeNodeAction(std::string name)
	: IBehaviorTreeNode::IBehaviorTreeNode(name) {
}

void CBehaviorTreeNodeAction::recalc(IBehaviorTree *behaviorTree) {
	int res = behaviorTree->execAction(name);
	if (res == Stay) {
		behaviorTree->setCurrent(this);
	}
	else {
		IBehaviorTreeNode *candidate = this;
		while (candidate->getParent() != nullptr) {
			IBehaviorTreeNode *parent = candidate->getParent();
			if (dynamic_cast<CBehaviorTreeNodeSequence*>(parent)) {
				if (candidate->getRight() != nullptr) {
					behaviorTree->setCurrent(candidate->getRight());
					break;
				}
				else candidate = parent;
			}
			else candidate = parent;
		}
		if (candidate->getParent() == nullptr) behaviorTree->setCurrent(nullptr);
	}
}