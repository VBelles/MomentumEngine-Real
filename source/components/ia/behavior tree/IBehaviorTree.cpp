#include "mcv_platform.h"
#include "IBehaviorTree.h"

IBehaviorTree::IBehaviorTree(std::string name) {
	this->name = name;
}

std::string IBehaviorTree::getName() {
	return name;
}

BehaviorTreeNode* IBehaviorTree::createNode(std::string name) {
	if (findNode(name)) {
		printf("Error: node %s already exists\n", name.c_str());
		return nullptr;
	}
	else {
		BehaviorTreeNode *btn = new BehaviorTreeNode(name);
		tree[name] = btn;
		return btn;
	}
}

BehaviorTreeNode* IBehaviorTree::findNode(std::string name) {
	if (tree.find(name) == tree.end()) {
		return nullptr;
	}
	else {
		return tree[name];
	}
}

BehaviorTreeNode* IBehaviorTree::createRoot(std::string rootName, BehaviorTreeNodeType type, BehaviorTreeCondition condition, BehaviorTreeAction action) {
	BehaviorTreeNode *rootNode = createNode(rootName);
	rootNode->setParent(nullptr);
	rootNode->setType(type);
	root = rootNode;
	if (condition != nullptr) addCondition(rootName, condition);
	if (action != nullptr) addAction(rootName, action);

	current = nullptr;
	return rootNode;
}

BehaviorTreeNode* IBehaviorTree::addChild(std::string parentName, std::string childName, BehaviorTreeNodeType type, BehaviorTreeCondition condition, BehaviorTreeAction action) {
	BehaviorTreeNode *parent = findNode(parentName);
	BehaviorTreeNode *child = createNode(childName);
	parent->addChild(child);
	child->setParent(parent);
	child->setType(type);
	if (condition != nullptr) addCondition(childName, condition);
	if (action != nullptr) addAction(childName, action);
	return child;
}

void IBehaviorTree::setCurrent(BehaviorTreeNode *newCurrent) {
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