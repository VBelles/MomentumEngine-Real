#include "mcv_platform.h"
#include "BehaviorTreeNode.h"
#include "IBehaviorTree.h"

BehaviorTreeNode::BehaviorTreeNode(std::string name) {
	this->name = name;
}

bool BehaviorTreeNode::isRoot() {
	return (parent == NULL);
}

void BehaviorTreeNode::setParent(BehaviorTreeNode *parent) {
	this->parent = parent;
}


void BehaviorTreeNode::setRight(BehaviorTreeNode *right) {
	this->right = right;
}

void BehaviorTreeNode::setType(int type) {
	this->type = type;
}

std::string BehaviorTreeNode::getName() {
	return name;
}

void BehaviorTreeNode::addChild(BehaviorTreeNode *child) {
	if (!children.empty()) { // if this node already had children, connect the last one to this
		children.back()->setRight(child);  // new one so the new one is to the RIGHT of the last one
	}
	children.push_back(child); // in any case, insert it
	child->right = NULL; // as we're adding from the right make sure right points to NULL
}

void BehaviorTreeNode::recalc(IBehaviorTree *behaviorTree) {
	switch (type) {
	case ACTION:
	{
		recalcAction(behaviorTree);
		break;
	}
	case RANDOM:
	{
		recalcRandom(behaviorTree);
		break;
	}
	case PRIORITY:
	{
		recalcPriority(behaviorTree);
		break;
	}
	case SEQUENCE:
	{
		recalcSequence(behaviorTree);
		break;
	}
	}
}

void BehaviorTreeNode::recalcRandom(IBehaviorTree *behaviorTree) {
	int r = rand() % children.size();
	children[r]->recalc(behaviorTree);
}

void BehaviorTreeNode::recalcSequence(IBehaviorTree *behaviorTree) {
	// begin the sequence...the inner node (action) will take care of the sequence
	// via the "setCurrent" mechanism
	children[0]->recalc(behaviorTree);
}

void BehaviorTreeNode::recalcPriority(IBehaviorTree *behaviorTree) {
	for (int i = 0; i < children.size(); i++) {
		if (behaviorTree->testCondition(children[i]->getName())) {
			children[i]->recalc(behaviorTree);
			break;
		}
	}
}

void BehaviorTreeNode::recalcAction(IBehaviorTree *behaviorTree) {
	// run the controller of this node
	int res = behaviorTree->execAction(name);
	// now, the next lines compute what's the NEXT node to use in the next frame...
	if (res == STAY) {
		behaviorTree->setCurrent(this);
		return;
	}// looping vs. on-shot actions
	 // climb tree iteratively, look for the next unfinished sequence to complete
	BehaviorTreeNode *cand = this;
	while (cand->parent != NULL) {
		BehaviorTreeNode *daddy = cand->parent;
		if (daddy->type == SEQUENCE)
			// oh we were doing a sequence. make sure we finished it!!!
		{
			if (cand->right != NULL) {
				behaviorTree->setCurrent(cand->right);
				break;
			}
			// sequence was finished (there is nobody on right). Go up to daddy.
			else cand = daddy;
		}
		// i'm not on a sequence, so keep moving up to the root of the BT
		else cand = daddy;
	}
	// if we've reached the root, means we can reset the traversal for next frame.
	if (cand->parent == NULL) behaviorTree->setCurrent(NULL);
}