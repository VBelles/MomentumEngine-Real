#include "mcv_platform.h"
#include "BehaviorTreeNodeAction.h"
#include "BehaviorTreeNodeSequence.h"
#include "IBehaviorTree.h"

BehaviorTreeNodeAction::BehaviorTreeNodeAction(std::string name)
	: IBehaviorTreeNode::IBehaviorTreeNode(name) {
}

void BehaviorTreeNodeAction::recalc(IBehaviorTree *behaviorTree) {
	int res = behaviorTree->execAction(name);
	if (res == Stay) {
		behaviorTree->setCurrent(this);
	}
	else {
		IBehaviorTreeNode *candidate = this;
		while (candidate->parent != nullptr) {
			IBehaviorTreeNode *parent = candidate->parent;
			if (dynamic_cast<BehaviorTreeNodeSequence*>(parent)) {
				if (candidate->right != nullptr) {
					behaviorTree->setCurrent(candidate->right);
					break;
				}
				else candidate = parent;
			}
			else candidate = parent;
		}
		if (candidate->parent == nullptr) behaviorTree->setCurrent(nullptr);
	}
}