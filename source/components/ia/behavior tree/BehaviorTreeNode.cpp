#include "mcv_platform.h"
#include "BehaviorTreeNode.h"
#include "IBehaviorTree.h"

CBehaviorTreeNode::CBehaviorTreeNode(std::string name) {
	this->name = name;
}

std::string CBehaviorTreeNode::getName() {
	return name;
}

bool CBehaviorTreeNode::isRoot() {
	return (parent == nullptr);
}

void CBehaviorTreeNode::setType(EBehaviorTreeNodeType type) {
	this->type = type;
}

void CBehaviorTreeNode::setParent(CBehaviorTreeNode *parent) {
	this->parent = parent;
}

void CBehaviorTreeNode::setRight(CBehaviorTreeNode *right) {
	this->right = right;
}

void CBehaviorTreeNode::addChild(CBehaviorTreeNode *child) {
	if (!children.empty()) { // if this node already had children, connect the last one to this
		children.back()->setRight(child);  // new one so the new one is to the RIGHT of the last one
	}
	children.push_back(child); // in any case, insert it
	child->right = nullptr; // as we're adding from the right make sure right points to NULL
}

void CBehaviorTreeNode::recalc(IBehaviorTree *behaviorTree) {
	switch (type) {
	case Random:
	{
		recalcRandom(behaviorTree);
		break;
	}
	case Sequence:
	{
		recalcSequence(behaviorTree);
		break;
	}
	case Priority:
	{
		recalcPriority(behaviorTree);
		break;
	}
	case Action:
	{
		recalcAction(behaviorTree);
		break;
	}
	}
}

void CBehaviorTreeNode::recalcRandom(IBehaviorTree *behaviorTree) {
	int random = rand() % children.size();
	children[random]->recalc(behaviorTree);
}

void CBehaviorTreeNode::recalcSequence(IBehaviorTree *behaviorTree) {
	children[0]->recalc(behaviorTree);
}

void CBehaviorTreeNode::recalcPriority(IBehaviorTree *behaviorTree) {
	for (int i = 0; i < children.size(); i++) {
		if (behaviorTree->testCondition(children[i]->getName())) {
			children[i]->recalc(behaviorTree);
			break;
		}
	}
}

void CBehaviorTreeNode::recalcAction(IBehaviorTree *behaviorTree) {
	int res = behaviorTree->execAction(name);
	if (res == Stay) {
		behaviorTree->setCurrent(this);
		return;
	}
	else {
		CBehaviorTreeNode *candidate = this;
		while (candidate->parent != nullptr) {
			CBehaviorTreeNode *parent = candidate->parent;
			if (parent->type == Sequence) {
				if (candidate->right != nullptr) {
					behaviorTree->setCurrent(candidate->right);
					break;
				}
				else candidate = parent;
			}
			else candidate = parent;
		}
		if (candidate->parent == nullptr) behaviorTree->setCurrent(nullptr);
	}
}