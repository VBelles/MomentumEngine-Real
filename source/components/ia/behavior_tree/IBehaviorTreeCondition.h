#pragma once

class IBehaviorTreeCondition {
public:
	virtual bool testCondition(float delta) = 0;
};