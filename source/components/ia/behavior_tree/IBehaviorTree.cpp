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
	PROFILE_FUNCTION("findNode");
	auto it = tree.find(name);
	if (it != tree.end()) {
		return it->second;
	}
	return nullptr;
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
	auto res = conditions.emplace(conditionName, condition);
	if (!res.second) {
		dbg("Error: node %s already has an action\n", conditionName.c_str());
	}
}

bool IBehaviorTree::testCondition(std::string conditionName, float delta) {
	PROFILE_FUNCTION("testCondition");
	auto it = conditions.find(conditionName);
	if (it == conditions.end()) {
		//dbg("ERROR: Missing node condition for node %s\n", conditionName.c_str());
		return true; // error: condition does not exist
	}
	return (this->*it->second)(delta);
}

void IBehaviorTree::addAction(std::string actionName, BehaviorTreeAction action) {
	auto res = actions.emplace(actionName, action);
	if (!res.second) {
		dbg("Error: node %s already has an action\n", actionName.c_str());
	}
}

int IBehaviorTree::execAction(std::string actionName, float delta) {
	PROFILE_FUNCTION("execAction");
	auto it = actions.find(actionName);
	if (it == actions.end()) {
		dbg("ERROR: Missing node action for node %s\n", actionName.c_str());
		return Leave; // error: action does not exist
	}
	return (this->*it->second)(delta);
}

void IBehaviorTree::recalc(float delta) {
	PROFILE_FUNCTION("Recalc");
	if (current == nullptr) {
		root->recalc(this, delta);
	}
	else {
		current->recalc(this, delta);
	}
}