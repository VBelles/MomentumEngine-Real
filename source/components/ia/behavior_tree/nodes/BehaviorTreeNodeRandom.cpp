#include "mcv_platform.h"
#include "BehaviorTreeNodeRandom.h"
#include "components/ia/behavior_tree/IBehaviorTree.h"
#include "components/ia/behavior_tree/IBehaviorTreeNew.h"

REGISTER_BTNODE("random", CBehaviorTreeNodeRandom);

CBehaviorTreeNodeRandom::CBehaviorTreeNodeRandom(std::string name)
	: IBehaviorTreeNode::IBehaviorTreeNode(name) {
}

void CBehaviorTreeNodeRandom::recalc(IBehaviorTree *behaviorTree, float delta) {
	float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	int i = 0;
	float prob = probability[i];
	while (i < children.size() - 1 && random >= prob) {
		i++;
		prob += probability[i];
	}
	children[i]->recalc(behaviorTree, delta);
}

void CBehaviorTreeNodeRandom::addChild(IBehaviorTreeNode *child) {
	IBehaviorTreeNode::addChild(child);
	float prob = 1.f / children.size();
	probability.push_back(prob);
	for (int i = 0; i < children.size() - 1; i++) {
		probability[i] = prob;
	}
}

void CBehaviorTreeNodeRandom::setProbability(std::vector<float> probabilities) {
	if (probabilities.size() == children.size()) {
		probability = probabilities;
	}
}

void CBehaviorTreeNodeRandom::recalc(IBehaviorTreeNew *behaviorTree, float delta) {
	float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	int i = 0;
	float prob = probability[i];
	while (i < children.size() - 1 && random >= prob) {
		i++;
		prob += probability[i];
	}
	children[i]->recalc(behaviorTree, delta);
}

void CBehaviorTreeNodeRandom::debugInMenu() {
	if (ImGui::TreeNode(getName().c_str())) {
		ImGui::Text("Type: Random");
		if (children.size() > 0 && ImGui::TreeNode("Children")) {
			for (int i = 0; i < children.size(); i++) {
				ImGui::Text("Probability: %f", probability[i]);
				children[i]->debugInMenu();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void CBehaviorTreeNodeRandom::debugInMenu(IBehaviorTreeNew* behaviorTree) {
	if (ImGui::TreeNode(getName().c_str())) {
		ImGui::Text("Type: Random");

		IBehaviorTreeCondition* condition = behaviorTree->getCondition(getName());
		ImGui::Text("Condition: %s\n", condition ? condition->getType().c_str() : "None");

		IBehaviorTreeAction* action = behaviorTree->getAction(getName());
		ImGui::Text("Action: %s\n", action ? action->getType().c_str() : "None");

		if (children.size() > 0 && ImGui::TreeNode("Children")) {
			for (int i = 0; i < children.size(); i++) {
				ImGui::Text("Probability: %f", probability[i]);
				children[i]->debugInMenu(behaviorTree);
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void CBehaviorTreeNodeRandom::load(const json& j) {
	if (j.count("probabilities")) {
		if (j["probabilities"].size() == children.size()) {
			probability.clear();
			for (auto& jProb : j["probabilities"]) {
				float prob = jProb;
				probability.push_back(prob);
			}
		}
	}
}