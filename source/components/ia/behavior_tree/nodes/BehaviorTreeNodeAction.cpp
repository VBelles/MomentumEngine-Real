#include "mcv_platform.h"
#include "BehaviorTreeNodeAction.h"
#include "BehaviorTreeNodeSequence.h"
#include "components/ia/behavior_tree/IBehaviorTree.h"

REGISTER_BTNODE("action", CBehaviorTreeNodeAction);

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


void CBehaviorTreeNodeAction::debugInMenu() {
	if (ImGui::TreeNode(getName().c_str())) {
		ImGui::Text("Type: Action");
		if (children.size() > 0 && ImGui::TreeNode("Children")) {
			for (auto& node : children) {
				node->debugInMenu();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void CBehaviorTreeNodeAction::debugInMenu(IBehaviorTree* behaviorTree) {
	if (ImGui::TreeNode(getName().c_str())) {
		ImGui::Text("Type: Action");

		IBehaviorTreeCondition* condition = behaviorTree->getCondition(getName());
		if (condition) {
			if (ImGui::TreeNode(("Condition: " + condition->getType()).c_str())) {
				condition->debugInMenu();
				ImGui::TreePop();
			}
		}
		else {
			ImGui::Text("Condition: None\n");
		}

		IBehaviorTreeAction* action = behaviorTree->getAction(getName());
		if (action) {
			if (ImGui::TreeNode(("Action: " + action->getType()).c_str())) {
				action->debugInMenu();
				ImGui::TreePop();
			}
		}
		else {
			ImGui::Text("Action: None\n");
		}

		if (children.size() > 0 && ImGui::TreeNode("Children")) {
			for (auto& node : children) {
				node->debugInMenu(behaviorTree);
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}
