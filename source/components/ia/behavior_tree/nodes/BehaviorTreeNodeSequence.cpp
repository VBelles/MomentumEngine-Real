#include "mcv_platform.h"
#include "BehaviorTreeNodeSequence.h"
#include "components/ia/behavior_tree/IBehaviorTree.h"

REGISTER_BTNODE("sequence", CBehaviorTreeNodeSequence);

CBehaviorTreeNodeSequence::CBehaviorTreeNodeSequence(std::string name)
	: IBehaviorTreeNode::IBehaviorTreeNode(name) {
}

void CBehaviorTreeNodeSequence::recalc(IBehaviorTree *behaviorTree, float delta) {
	children[0]->recalc(behaviorTree, delta);
}

void CBehaviorTreeNodeSequence::debugInMenu() {
	if (ImGui::TreeNode(getName().c_str())) {
		ImGui::Text("Type: Sequence");
		if (children.size() > 0 && ImGui::TreeNode("Children")) {
			for (auto& node : children) {
				node->debugInMenu();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void CBehaviorTreeNodeSequence::debugInMenu(IBehaviorTree* behaviorTree) {
	if (ImGui::TreeNode(getName().c_str())) {
		ImGui::Text("Type: Sequence");

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
