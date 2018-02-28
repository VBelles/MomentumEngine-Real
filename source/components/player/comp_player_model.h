#pragma once

#include "components/comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "states/IActionState.h"
#include "PowerGauge.h"


struct PowerStats {
	float maxHorizontalSpeed = 0.f;
	float rotationSpeed = 0.f;
	float fallingMultiplier = 1.1f;
	float longGravityMultiplier = 1.f;
	float normalGravityMultiplier = 1.f;
	float currentGravityMultiplier = 1.f;
	float maxVelocityVertical = 30.f;
	float acceleration = 30.f;
	float airAcceleration = 40.f;
	float shortHopVelocity = 6.f;
	VEC3 jumpVelocityVector = { 0.f, 8.f, 0.f };
	VEC3 longJumpVelocityVector = { 0.f, 6.f, 12.f };
};


class TCompPlayerModel : public TCompBase, PowerGauge::PowerListener {
	DECL_SIBLING_ACCESS();

public:
	int frame = 0;
	enum ActionStates{
		Idle, JumpSquat, GhostJumpSquat, GhostJumpWindow,
		Run, AirborneNormal, JumpSquatLong, AirborneLong,
		GhostJumpSquatLong, StrongAttack, FallingAttack,
		VerticalLauncher, GrabHigh, GrabLong, TurnAround
	};
	IActionState* movementState;
	IActionState* attackState;
	
	bool lockMovementState = false;
	bool lockWalk = false;
	bool lockAttackState = false;
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void SetMovementState(ActionStates newState);
	void SetAttackState(ActionStates newState);
	void update(float dt);
	void UpdateMovement(float dt, VEC3 deltaMovement);
	void SetMovementInput(VEC2 input, float delta);
	void JumpButtonPressed();
	void JumpButtonReleased();
	void LongJumpButtonPressed();
	void FastAttackButtonPressed();
	void FastAttackButtonReleased();
	void StrongAttackButtonPressed();
	void StrongAttackButtonReleased();
	void CenterCameraButtonPressed();
	void ReleasePowerButtonPressed();
	void GainPowerButtonPressed();
	bool IsAttackFree();

	void OnAttackHit(const TMsgAttackHit& msg);
	void OnHitboxEnter(const TMsgHitboxEnter& msg);
	void OnGainPower(const TMsgGainPower& msg);

	TCompTransform* GetTransform() { return myTransform; }
	TCompCamera* GetCamera() { return currentCamera; }
	TCompCollider* GetCollider() { return collider; }
	VEC3* GetAccelerationVector() { return &accelerationVector; }
	VEC3* GetVelocityVector() { return &velocityVector; }
	
	PowerStats* GetPowerStats();
	
	bool isGrounded = false;
	bool isTouchingCeiling = false;

	void OnLevelChange(int powerLevel);

private:
	VEC3 deltaMovement;
	TCompTransform *myTransform;
	TCompCamera *currentCamera;
	TCompCollider* collider;
	VEC3 accelerationVector = {0.f, 0.f, 0.f};
	VEC3 velocityVector = { 0.f, 0.f, 0.f };
	float gravity = 0;

	PowerStats* ssj1;
	PowerStats* ssj2;
	PowerStats* ssj3;
	PowerStats* currentPowerStats;

	int hp = 3;
	int maxHp = 3;
    int chrysalis = 0;
    int chrysalisTarget = 5;
 
    void OnGroupCreated(const TMsgEntitiesGroupCreated& msg);
    void OnCollect(const TMsgCollect& msg);

	PowerStats* loadPowerStats(const json& j);

	PowerGauge* powerGauge;

	std::map<ActionStates, IActionState*> movementStates;
	std::map<ActionStates, IActionState*> attackStates;

	CHandle strongAttackHitbox;
	CHandle fallingAttackHitbox;
	CHandle verticalLauncherHitbox;
	CHandle grabHitbox;
};
