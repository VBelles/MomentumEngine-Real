#include "mcv_platform.h"
#include "IBehaviorTree.h"

IBehaviorTree::IBehaviorTree() {
}

IBehaviorTreeNode* IBehaviorTree::createNode(std::string name, EBehaviorTreeNodeType type) {
	if (findNode(name)) {
		printf("Error: node %s already exists\n", name.c_str());
		return nullptr;
	}
	else {
		IBehaviorTreeNode *behaviorTreeNode = nullptr;
		if (type == Random) {
			behaviorTreeNode = new CBehaviorTreeNodeRandom(name);
		}
		else if (type == Sequence) {
			behaviorTreeNode = new CBehaviorTreeNodeSequence(name);
		}
		else if (type == Priority) {
			behaviorTreeNode = new CBehaviorTreeNodePriority(name);
		}
		else if (type == Action) {
			behaviorTreeNode = new CBehaviorTreeNodeAction(name);
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

bool IBehaviorTree::falseCondition(float delta) {
	return false;
}
bool IBehaviorTree::trueCondition(float delta) {
	return true;
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

bool IBehaviorTree::testCondition(std::string conditionName, float delta) {
	if (conditions.find(conditionName) == conditions.end()) {
		return true;	// error: no condition defined, we assume TRUE
	}
	else {
		return (this->*conditions[conditionName])(delta);
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

int IBehaviorTree::execAction(std::string actionName, float delta) {
	if (actions.find(actionName) == actions.end()) {
		printf("ERROR: Missing node action for node %s\n", actionName.c_str());
		return Leave; // error: action does not exist
	}
	else {
		return (this->*actions[actionName])(delta);
	}
}

void IBehaviorTree::recalc(float delta) {
	if (current == nullptr) {
		root->recalc(this, delta);
	}
	else {
		current->recalc(this, delta);
	}
}