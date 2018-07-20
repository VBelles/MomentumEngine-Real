#include "mcv_platform.h"
#include "IBehaviorTreeNode.h"
#include "components/ia/behavior_tree/IBehaviorTree.h"
#include "components/ia/behavior_tree/IBehaviorTreeNew.h"

IBehaviorTreeNode::IBehaviorTreeNode(std::string name) {
	this->name = name;
}

std::string IBehaviorTreeNode::getName() {
	return name;
}

IBehaviorTreeNode* IBehaviorTreeNode::getParent() {
	return parent;
}

IBehaviorTreeNode* IBehaviorTreeNode::getRight() {
	return right;
}

bool IBehaviorTreeNode::isRoot() {
	return (parent == nullptr);
}

void IBehaviorTreeNode::setParent(IBehaviorTreeNode *parent) {
	this->parent = parent;
}

void IBehaviorTreeNode::setRight(IBehaviorTreeNode *right) {
	this->right = right;
}

void IBehaviorTreeNode::addChild(IBehaviorTreeNode *child) {
	if (!children.empty()) { // if this node already had children, connect the last one to this
		children.back()->setRight(child);  // new one so the new one is to the RIGHT of the last one
	}
	children.push_back(child); // in any case, insert it
	child->right = nullptr; // as we're adding from the right make sure right points to NULL
}

void IBehaviorTreeNode::debugInMenu() {
	if (ImGui::TreeNode(getName().c_str())) {
		if (children.size() > 0 && ImGui::TreeNode("Children")) {
			for (auto& node : children) {
				node->debugInMenu();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void IBehaviorTreeNode::debugInMenu(IBehaviorTreeNew* behaviorTree) {
	if (ImGui::TreeNode(getName().c_str())) {
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