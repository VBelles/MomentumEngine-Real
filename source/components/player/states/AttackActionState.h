#pragma once

class IActionState;
class StateManager;
class TCompHitboxes;

class AttackActionState{
protected:

	enum AttackPhases {
		Launch, Startup, Active, Recovery
	};

	CTimer timer;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;

	AttackPhases phase = AttackPhases::Startup;

	std::string hitbox;
	StateManager* _stateManager;

	TCompHitboxes* getAttackHitboxes();
	//virtual bool isChangingBaseState() = 0;

public:
	AttackActionState(StateManager* stateManager);

	void update(float delta);
	void onStateEnter(IActionState* lastState);
	void onStateExit(IActionState* nextState);
};