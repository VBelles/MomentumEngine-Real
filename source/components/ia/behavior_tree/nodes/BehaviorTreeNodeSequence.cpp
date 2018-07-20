#include "mcv_platform.h"
#include "BehaviorTreeNodeSequence.h"
#include "components/ia/behavior_tree/IBehaviorTree.h"
#include "components/ia/behavior_tree/IBehaviorTreeNew.h"

REGISTER_BTNODE("sequence", CBehaviorTreeNodeSequence);

CBehaviorTreeNodeSequence::CBehaviorTreeNodeSequence(std::string name)
	: IBehaviorTreeNode::IBehaviorTreeNode(name) {
}

void CBehaviorTreeNodeSequence::recalc(IBehaviorTree *behaviorTree, float delta) {
	children[0]->recalc(behaviorTree, delta);
}

void CBehaviorTreeNodeSequence::recalc(IBehaviorTreeNew *behaviorTree, float delta) {
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

void CBehaviorTreeNodeSequence::debugInMenu(IBehaviorTreeNew* behaviorTree) {
	if (ImGui::TreeNode(getName().c_str())) {
		ImGui::Text("Type: Sequence");

		IBehaviorTreeCondition* condition = behaviorTree->getCondition(getName());
		ImGui::Text("Condition: %s\n", condition ? condition->getType().c_str() : "None");

		IBehaviorTreeAction* action = behaviorTree->getAction(getName());
		ImGui::Text("Action: %s\n", action ? action->getType().c_str() : "None");

		if (children.size() > 0 && ImGui::TreeNode("Children")) {
			for (auto& node : children) {
				node->debugInMenu(behaviorTree);
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}