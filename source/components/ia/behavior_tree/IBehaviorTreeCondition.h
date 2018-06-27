#pragma once

#include "components/ia/enemies/Enemy.h"

class IBehaviorTreeCondition {
public:
	virtual bool testCondition(float delta) = 0;
	friend class Enemy;
};