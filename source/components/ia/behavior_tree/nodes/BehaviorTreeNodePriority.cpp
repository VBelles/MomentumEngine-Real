#include "mcv_platform.h"
#include "BehaviorTreeNodePriority.h"
#include "components/ia/behavior_tree/IBehaviorTree.h"
#include "components/ia/behavior_tree/IBehaviorTreeNew.h"

REGISTER_BTNODE("priority", CBehaviorTreeNodePriority);

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

void CBehaviorTreeNodePriority::recalc(IBehaviorTreeNew *behaviorTree, float delta) {
	for (int i = 0; i < children.size(); i++) {
		if (behaviorTree->testCondition(children[i]->getName(), delta)) {
			children[i]->recalc(behaviorTree, delta);
			break;
		}
	}
}

void CBehaviorTreeNodePriority::debugInMenu() {
	if (ImGui::TreeNode(getName().c_str())) {
		ImGui::Text("Type: Priority");
		for (auto& node : children) {
			node->debugInMenu();
		}
		ImGui::TreePop();
	}
}