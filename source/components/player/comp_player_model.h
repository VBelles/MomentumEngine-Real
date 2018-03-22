#pragma once

#include "components/comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "states/IActionState.h"
#include "comp_power_gauge.h"

class PlayerFilterCallback;
//typedef TCompPlayerModel::ActionStates ActionStates;

struct PowerStats {
	float maxHorizontalSpeed = 0.f;
	float rotationSpeed = 0.f;
	float landingLag = 0.f;
	float fallingMultiplier = 1.1f;
	float longGravityMultiplier = 1.f;
	float normalGravityMultiplier = 1.f;
	float currentGravityMultiplier = 1.f;
	float maxVelocityVertical = 30.f;
	float acceleration = 30.f;
	float deceleration = 40.f;
	float airAcceleration = 40.f;
	float shortHopVelocity = 6.f;
	VEC3 jumpVelocityVector = { 0.f, 8.f, 0.f };
	VEC3 longJumpVelocityVector = { 0.f, 6.f, 12.f };
};

class TCompPlayerModel : public TCompBase {
	DECL_SIBLING_ACCESS();

public:
	int frame = 0;
	enum ActionStates {
		Idle, JumpSquat, GhostJumpSquat, GhostJumpWindow,
		Run, Walk, AirborneNormal, JumpSquatLong, AirborneLong,
		GhostJumpSquatLong, FastAttack, StrongAttack, FallingAttack,
		HorizontalLauncher, VerticalLauncher, GrabHigh, GrabLong,
		PropelHigh, PropelLong, TurnAround, Landing, LandingFallingAttack,
		HuggingWall, HuggingWallJumpSquat, HuggingWallLongJumpSquat, AirborneWallJump
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
	void OnOutOfBounds(const TMsgOutOfBounds& msg);

	TCompTransform* GetTransform() { return myTransformHandle; }
	TCompCollider* GetCollider() { return colliderHandle; }
	PxCapsuleController* GetController() { return static_cast<PxCapsuleController*>(GetCollider()->controller); }
	TCompCamera* GetCamera();
	TCompPowerGauge* GetPowerGauge() { return powerGaugeHandle; }
	
	VEC3* GetAccelerationVector() { return &accelerationVector; }
	VEC3* GetVelocityVector() { return &velocityVector; }
	float GetGravity() { return currentGravity; }
	void SetGravity(float newGravity) { currentGravity = newGravity; }
	void SetGravityMultiplier(float multiplier) { currentGravity = baseGravity * multiplier; }
	void ResetGravity() { currentGravity = baseGravity; }

	PowerStats* GetPowerStats();

	bool isInState(ActionStates state);
	template <typename T>
	T GetMovementState(TCompPlayerModel::ActionStates state) { return static_cast<T>(movementStates[state]); }
	template <typename T >
	T GetAttackState(TCompPlayerModel::ActionStates state) { return static_cast<T>(attackStates[state]); }
	
	bool isGrounded = false;
	bool isTouchingCeiling = false;
	bool isAttachedToPlatform = false;
	float maxVerticalSpeed = 0.f;

	float walkingSpeed = 0.f;

	float huggingWallMinPitch = deg2rad(-25);
	float huggingWallMaxPitch = deg2rad(5);
	float huggingWallAttachThreshold = 0.8f;


	CHandle grabTarget;

	

private:
	VEC3 deltaMovement;
	CHandle myTransformHandle;
	CHandle colliderHandle;
	CHandle powerGaugeHandle;

	VEC3 accelerationVector = {0.f, 0.f, 0.f};
	VEC3 velocityVector = { 0.f, 0.f, 0.f };
	float baseGravity = 0.f;
	float currentGravity = 0.f;

	PowerStats* ssj1;
	PowerStats* ssj2;
	PowerStats* ssj3;
	PowerStats* currentPowerStats;

	int hp = 8;
	int maxHp = 8;
    int chrysalis = 0;
    int chrysalisTarget = 5;
	bool showVictoryDialog = false;

	CTimer dialogTimer;
	float dialogTime = 15.0f;
 
    void OnGroupCreated(const TMsgEntitiesGroupCreated& msg);
    void OnCollect(const TMsgCollect& msg);
	void OnShapeHit(const TMsgOnShapeHit & msg);
	void OnLevelChange(const TMsgPowerLvlChange& msg);

	PowerStats* loadPowerStats(const json& j);


	PlayerFilterCallback* playerFilterCallback;

	std::map<ActionStates, IActionState*> movementStates;
	std::map<ActionStates, IActionState*> attackStates;
	ActionStates nextMovementState;
	ActionStates nextAttackState;

	CHandle strongAttackHitbox;
	CHandle fallingAttackHitbox;
	CHandle fallingAttackLandingHitbox;
	CHandle verticalLauncherHitbox;
	CHandle grabHitbox;


	VEC3 respawnPosition = {0, 3, 0}; //Asegurar que esta posición estará libre


	void ChangeMovementState(ActionStates newState);
	void ChangeAttackState(ActionStates newState);
	void OnDead();
	
	void ApplyGravity(float delta);
	
};
