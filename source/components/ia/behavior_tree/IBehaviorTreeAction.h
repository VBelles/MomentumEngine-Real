#pragma once

class IBehaviorTreeAction {
public:
	IBehaviorTreeAction() {}
	virtual int execAction(float delta) = 0;
};