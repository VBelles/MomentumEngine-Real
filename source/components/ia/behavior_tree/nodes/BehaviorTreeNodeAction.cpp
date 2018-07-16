#include "mcv_platform.h"
#include "BehaviorTreeNodeAction.h"
#include "BehaviorTreeNodeSequence.h"
#include "components/ia/behavior_tree/IBehaviorTree.h"
#include "components/ia/behavior_tree/IBehaviorTreeNew.h"

CBehaviorTreeNodeAction::CBehaviorTreeNodeAction(std::string name)
	: IBehaviorTreeNode::IBehaviorTreeNode(name) {
}

void CBehaviorTreeNodeAction::recalc(IBehaviorTree *behaviorTree, float delta) {
	int res = behaviorTree->execAction(name, delta);
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

void CBehaviorTreeNodeAction::recalc(IBehaviorTreeNew *behaviorTree, float delta) {
	int res = behaviorTree->execAction(name, delta);
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

void CBehaviorTreeNodeAction::debugInMenu() {
	if (ImGui::TreeNode(getName().c_str())) {
		ImGui::Text("Type: Action");
		for (auto& node : children) {
			node->debugInMenu();
		}
		ImGui::TreePop();
	}
}