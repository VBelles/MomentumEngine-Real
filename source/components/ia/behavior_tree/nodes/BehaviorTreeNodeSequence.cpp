#include "mcv_platform.h"
#include "BehaviorTreeNodeSequence.h"
#include "components/ia/behavior_tree/IBehaviorTree.h"
#include "components/ia/behavior_tree/IBehaviorTreeNew.h"

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
		for (auto& node : children) {
			node->debugInMenu();
		}
		ImGui::TreePop();
	}
}