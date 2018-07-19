#include "mcv_platform.h"
#include "IBehaviorTreeNew.h"

IBehaviorTreeNew::IBehaviorTreeNew() {
}

IBehaviorTreeNew::~IBehaviorTreeNew() {
	clear();
}

void IBehaviorTreeNew::clear() {
	for (IBehaviorTreeCondition* condition : allConditions) {
		SAFE_DELETE(condition);
	}
	for (IBehaviorTreeAction* action : allActions) {
		SAFE_DELETE(action);
	}
	for (auto& pair : tree) {
		SAFE_DELETE(pair.second);
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

IBehaviorTreeNode* IBehaviorTreeNew::createNode(std::string name, std::string type) {
	if (findNode(name)) {
		dbg("Error: node %s already exists\n", name.c_str());
		return nullptr;
	}
	else {
		IBehaviorTreeNode *behaviorTreeNode = BTNodeFactory::get().create(type, name);

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

IBehaviorTreeNode* IBehaviorTreeNew::createRoot(std::string rootName, std::string type, IBehaviorTreeCondition* condition, IBehaviorTreeAction* action) {
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

IBehaviorTreeNode* IBehaviorTreeNew::addChild(
	std::string parentName, std::string childName, std::string type, IBehaviorTreeCondition* condition, IBehaviorTreeAction* action
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

void IBehaviorTreeNew::debugInMenu() {
	if (ImGui::TreeNode("Behavior tree")) {
		root->debugInMenu();
		ImGui::TreePop();
	}
}

IBehaviorTreeCondition* IBehaviorTreeNew::loadCondition(const json& j) {
	assert(j.count("type"));
	std::string conditionType = j.value("type", "");
	IBehaviorTreeCondition* condition = BTConditionFactory::get().create(conditionType);
	if (condition) {
		condition->load(this, j);
	}
	return condition;
}

IBehaviorTreeAction* IBehaviorTreeNew::loadAction(const json& j) {
	assert(j.count("type"));
	std::string actionType = j.value("type", "");
	IBehaviorTreeAction* action = BTActionFactory::get().create(actionType);
	if (action) {
		action->load(this, j);
	}
	return action;
}

void IBehaviorTreeNew::loadNode(std::string parent, const json& j) {
	std::string nodeName = j.value("name", "");
	std::string nodeType = j.value("type", "");

	IBehaviorTreeCondition* nodeCondition = nullptr;
	if (j.count("condition")) {
		nodeCondition = loadCondition(j["condition"]);
	}

	IBehaviorTreeAction* nodeAction = nullptr;
	if (j.count("action")) {
		nodeAction = loadAction(j["action"]);
	}

	IBehaviorTreeNode* node = addChild(parent, nodeName, nodeType, nodeCondition, nodeAction);
	assert(node);

	if (j.count("children")) {
		for (auto& jNode : j["children"]) {
			loadNode(nodeName, jNode);
		}
	}

	node->load(j);
}

void IBehaviorTreeNew::load(const json& j) {
	assert(j.count("root"));

	const json& jRoot = j["root"];
	std::string rootName = jRoot.value("name", "");
	std::string rootType = jRoot.value("type", "");

	IBehaviorTreeCondition* rootCondition = nullptr;
	if (jRoot.count("condition")) {
		rootCondition = loadCondition(jRoot["condition"]);
	}

	IBehaviorTreeAction* rootAction = nullptr;
	if (jRoot.count("action")) {
		rootAction = loadAction(jRoot["action"]);
	}

	IBehaviorTreeNode* node = createRoot(rootName, rootType, rootCondition, rootAction);
	assert(node);

	if (jRoot.count("children")) {
		for (auto& jNode : jRoot["children"]) {
			loadNode(rootName, jNode);
		}
	}

	node->load(j);
}
