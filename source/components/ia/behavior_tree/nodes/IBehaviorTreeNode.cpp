#include "mcv_platform.h"
#include "IBehaviorTreeNode.h"
#include "components/ia/behavior_tree/IBehaviorTree.h"
#include "components/ia/behavior_tree/IBehaviorTree.h"

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

void IBehaviorTreeNode::debugInMenu(IBehaviorTree* behaviorTree) {
	if (ImGui::TreeNode(getName().c_str())) {
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