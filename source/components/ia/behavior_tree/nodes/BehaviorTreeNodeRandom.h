#pragma once

#include "IBehaviorTreeNode.h"

class CBehaviorTreeNodeRandom : public IBehaviorTreeNode {
private:
	std::vector<float> probability;

public:
	CBehaviorTreeNodeRandom(std::string name);

	void recalc(IBehaviorTree *behaviorTree, float delta = 0.f) override;
	void recalc(IBehaviorTreeNew *behaviorTree, float delta = 0.f) override;
	void addChild(IBehaviorTreeNode *child) override;
	void setProbability(std::vector<float> probabilities);

	virtual void debugInMenu();
	void load(const json& j) override;
};