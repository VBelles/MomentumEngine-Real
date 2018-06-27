#pragma once

#include "components/ia/enemies/Enemy.h"

class IBehaviorTreeAction {
public:
	IBehaviorTreeAction() {}
	virtual int execAction(float delta) = 0;
	friend class Enemy;
};