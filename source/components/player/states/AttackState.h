#pragma once


class IActionState;
class StateManager;
class TCompHitboxes;


class AttackState{
protected:

	enum AttackPhases {
		Launch, Startup, Active, Recovery
	};

	CTimer timer;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;

	CTimer cancelTimer;
	float cancelableTime;
	float interruptibleTime;

	CTimer invulnerableTimer;
	float invulnerabilityStartTime;
	float invulnerabilityEndTime;
	float superarmorStartTime;
	float superarmorEndTime;
	AttackPhases phase = AttackPhases::Startup;

	std::string hitbox;
	StateManager* _stateManager;

	TCompHitboxes* getAttackHitboxes();

public:
	AttackState(StateManager* stateManager);

	void update(float delta);
	void onStateEnter(IActionState* lastState);
	void onStateExit(IActionState* nextState);
	bool isCancelable();
	bool isInterruptible();
	bool hasInvulnerability();
	bool hasSuperArmor();
};