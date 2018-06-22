#include "mcv_platform.h"
#include "IBehaviorTreeNode.h"
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