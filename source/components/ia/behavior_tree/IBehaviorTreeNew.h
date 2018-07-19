#pragma once

#include "nodes/IBehaviorTreeNode.h"
#include "nodes/BehaviorTreeNodeRandom.h"
#include "nodes/BehaviorTreeNodeSequence.h"
#include "nodes/BehaviorTreeNodePriority.h"
#include "nodes/BehaviorTreeNodeAction.h"
#include "components/ia/behavior_tree/IBehaviorTreeAction.h"
#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class IBehaviorTreeNew {
protected:
	std::unordered_map<std::string, IBehaviorTreeNode *> tree;
	std::unordered_map<std::string, IBehaviorTreeCondition*> conditions;
	std::set<IBehaviorTreeCondition*> allConditions;
	std::unordered_map<std::string, IBehaviorTreeAction*> actions;
	std::set<IBehaviorTreeAction*> allActions;

	IBehaviorTreeNode* root;
	IBehaviorTreeNode* current;

	IBehaviorTreeNode* createNode(std::string name, EBehaviorTreeNodeType type);
	IBehaviorTreeNode* createNode(std::string name, std::string type);
	IBehaviorTreeNode* findNode(std::string name);

	bool falseCondition(float delta = 0.f);
	bool trueCondition(float delta = 0.f);

	IBehaviorTreeCondition* loadCondition(const json& j);
	IBehaviorTreeAction* loadAction(const json& j);
	void loadNode(std::string parent, const json& j);

public:
	IBehaviorTreeNew();
	virtual ~IBehaviorTreeNew();

	void clear();

	IBehaviorTreeNode* createRoot(std::string rootName, EBehaviorTreeNodeType type, IBehaviorTreeCondition* condition, IBehaviorTreeAction* action);
	IBehaviorTreeNode* createRoot(std::string rootName, std::string type, IBehaviorTreeCondition* condition, IBehaviorTreeAction* action);
	IBehaviorTreeNode* addChild(std::string parentName, std::string childName, EBehaviorTreeNodeType type, IBehaviorTreeCondition* condition, IBehaviorTreeAction* action);
	IBehaviorTreeNode* addChild(std::string parentName, std::string childName, std::string type, IBehaviorTreeCondition* condition, IBehaviorTreeAction* action);

	void setCurrent(IBehaviorTreeNode *newCurrent);

	void addCondition(std::string conditionName, IBehaviorTreeCondition* condition);
	bool testCondition(std::string conditionName, float delta = 0.f);

	void addAction(std::string actionName, IBehaviorTreeAction* action);
	int execAction(std::string actionName, float delta = 0.f);

	virtual void recalc(float delta = 0.f);

	virtual void debugInMenu();
	virtual void load(const json& j);
};