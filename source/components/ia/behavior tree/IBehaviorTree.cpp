#include "mcv_platform.h"
#include "IBehaviorTree.h"
#include "BehaviorTreeNodeRandom.h"
#include "BehaviorTreeNodeSequence.h"
#include "BehaviorTreeNodePriority.h"
#include "BehaviorTreeNodeAction.h"

IBehaviorTree::IBehaviorTree(std::string name) {
	this->name = name;
}

std::string IBehaviorTree::getName() {
	return name;
}

IBehaviorTreeNode* IBehaviorTree::createNode(std::string name, EBehaviorTreeNodeType type) {
	if (findNode(name)) {
		printf("Error: node %s already exists\n", name.c_str());
		return nullptr;
	}
	else {
		IBehaviorTreeNode *behaviorTreeNode;
		if (type == Random) {
			behaviorTreeNode = new BehaviorTreeNodeRandom(name);
		}
		else if (type == Sequence) {
			behaviorTreeNode = new BehaviorTreeNodeSequence(name);
		}
		else if (type == Priority) {
			behaviorTreeNode = new BehaviorTreeNodePriority(name);
		}
		else if (type == Action) {
			behaviorTreeNode = new BehaviorTreeNodeAction(name);
		}

		tree[name] = behaviorTreeNode;
		return behaviorTreeNode;
	}
}

IBehaviorTreeNode* IBehaviorTree::findNode(std::string name) {
	if (tree.find(name) == tree.end()) {
		return nullptr;
	}
	else {
		return tree[name];
	}
}

IBehaviorTreeNode* IBehaviorTree::createRoot(std::string rootName, EBehaviorTreeNodeType type, BehaviorTreeCondition condition, BehaviorTreeAction action) {
	IBehaviorTreeNode *rootNode = createNode(rootName, type);
	rootNode->setParent(nullptr);
	root = rootNode;
	if (condition != nullptr) addCondition(rootName, condition);
	if (action != nullptr) addAction(rootName, action);

	current = nullptr;
	return rootNode;
}

IBehaviorTreeNode* IBehaviorTree::addChild(
	std::string parentName, std::string childName, EBehaviorTreeNodeType type, BehaviorTreeCondition condition, BehaviorTreeAction action
) {
	IBehaviorTreeNode *parent = findNode(parentName);
	IBehaviorTreeNode *child = createNode(childName, type);
	parent->addChild(child);
	child->setParent(parent);
	if (condition != nullptr) addCondition(childName, condition);
	if (action != nullptr) addAction(childName, action);
	return child;
}

void IBehaviorTree::setCurrent(IBehaviorTreeNode *newCurrent) {
	current = newCurrent;
}

void IBehaviorTree::addCondition(std::string conditionName, BehaviorTreeCondition condition) {
	if (conditions.find(conditionName) != conditions.end()) {
		printf("Error: node %s already has a condition\n", conditionName.c_str());
	}
	else {
		conditions[conditionName] = condition;
	}
}

bool IBehaviorTree::testCondition(std::string conditionName) {
	if (conditions.find(conditionName) == conditions.end()) {
		return true;	// error: no condition defined, we assume TRUE
	}
	else {
		return (this->*conditions[conditionName])();
	}
}

void IBehaviorTree::addAction(std::string actionName, BehaviorTreeAction action) {
	if (actions.find(actionName) != actions.end()) {
		printf("Error: node %s already has an action\n", actionName.c_str());
	}
	else {
		actions[actionName] = action;
	}
}

int IBehaviorTree::execAction(std::string actionName) {
	if (actions.find(actionName) == actions.end()) {
		printf("ERROR: Missing node action for node %s\n", actionName.c_str());
		return Leave; // error: action does not exist
	}
	else {
		return (this->*actions[actionName])();
	}
}

void IBehaviorTree::recalc() {
	if (current == nullptr) {
		root->recalc(this);
	}
	else {
		current->recalc(this);
	}
}