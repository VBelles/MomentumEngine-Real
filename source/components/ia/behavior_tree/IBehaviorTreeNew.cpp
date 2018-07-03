#include "mcv_platform.h"
#include "IBehaviorTreeNew.h"

IBehaviorTreeNew::IBehaviorTreeNew() {
}

IBehaviorTreeNew::~IBehaviorTreeNew() {
	for (auto& pair : tree) {
		SAFE_DELETE(pair.second);
	}
	for (IBehaviorTreeCondition* condition : allConditions) {
		SAFE_DELETE(condition);
	}
	for (IBehaviorTreeAction* action : allActions) {
		SAFE_DELETE(action);
	}
	tree.clear();
	allConditions.clear();
	conditions.clear();
	allActions.clear();
	actions.clear();
}

IBehaviorTreeNode* IBehaviorTreeNew::createNode(std::string name, EBehaviorTreeNodeType type) {
	if (findNode(name)) {
		dbg("Error: node %s already exists\n", name.c_str());
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

IBehaviorTreeNode* IBehaviorTreeNew::findNode(std::string name) {
	PROFILE_FUNCTION("findNode");
	auto it = tree.find(name);
	if (it != tree.end()) {
		return it->second;
	}
	return nullptr;
}

bool IBehaviorTreeNew::falseCondition(float delta) {
	return false;
}
bool IBehaviorTreeNew::trueCondition(float delta) {
	return true;
}

IBehaviorTreeNode* IBehaviorTreeNew::createRoot(std::string rootName, EBehaviorTreeNodeType type, IBehaviorTreeCondition* condition, IBehaviorTreeAction* action) {
	IBehaviorTreeNode *rootNode = createNode(rootName, type);
	rootNode->setParent(nullptr);
	root = rootNode;
	if (condition != nullptr) addCondition(rootName, condition);
	if (action != nullptr) addAction(rootName, action);

	current = nullptr;
	return rootNode;
}

IBehaviorTreeNode* IBehaviorTreeNew::addChild(
	std::string parentName, std::string childName, EBehaviorTreeNodeType type, IBehaviorTreeCondition* condition, IBehaviorTreeAction* action
) {
	IBehaviorTreeNode *parent = findNode(parentName);
	IBehaviorTreeNode *child = createNode(childName, type);
	parent->addChild(child);
	child->setParent(parent);
	if (condition != nullptr) addCondition(childName, condition);
	if (action != nullptr) addAction(childName, action);
	return child;
}

void IBehaviorTreeNew::setCurrent(IBehaviorTreeNode *newCurrent) {
	current = newCurrent;
}

void IBehaviorTreeNew::addCondition(std::string conditionName, IBehaviorTreeCondition* condition) {
	auto res = conditions.emplace(conditionName, condition);
	allConditions.insert(condition);
	if (!res.second) {
		dbg("Error: node %s already has an action\n", conditionName.c_str());
	}
}

bool IBehaviorTreeNew::testCondition(std::string conditionName, float delta) {
	PROFILE_FUNCTION("testCondition");
	auto it = conditions.find(conditionName);
	if (it == conditions.end()) {
		//dbg("ERROR: Missing node condition for node %s\n", conditionName.c_str());
		return true; // error: condition does not exist
	}
	return it->second->testCondition(delta);
}

void IBehaviorTreeNew::addAction(std::string actionName, IBehaviorTreeAction* action) {
	auto res = actions.emplace(actionName, action);
	allActions.insert(action);
	if (!res.second) {
		dbg("Error: node %s already has an action\n", actionName.c_str());
	}
	else {
		dbg("Node action %s added\n", actionName.c_str());
	}
}

int IBehaviorTreeNew::execAction(std::string actionName, float delta) {
	PROFILE_FUNCTION("execAction");
	auto it = actions.find(actionName);
	if (it == actions.end()) {
		dbg("ERROR: Missing node action for node %s\n", actionName.c_str());
		return Leave; // error: action does not exist
	}
	return it->second->execAction(delta);
}

void IBehaviorTreeNew::recalc(float delta) {
	PROFILE_FUNCTION("Recalc");
	if (current == nullptr) {
		root->recalc(this, delta);
	}
	else {
		current->recalc(this, delta);
	}
}