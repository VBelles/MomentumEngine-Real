#pragma once

#include "components/comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "states/IActionState.h"
#include "comp_power_gauge.h"

class PlayerFilterCallback;
struct TMsgCollect;

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
	float springJumpVelocity = 23.f;
	float plummetTime = 0.5f;
	VEC3 jumpVelocityVector = { 0.f, 8.f, 0.f };
	VEC3 longJumpVelocityVector = { 0.f, 6.f, 12.f };
	VEC3 wallJumpVelocityVector = { 0.f, 20.f, 7.f };
};

class TCompPlayerModel : public TCompBase {
	DECL_SIBLING_ACCESS();

public:
	
	enum ActionStates {
		Idle, JumpSquat, GhostJumpSquat, GhostJumpWindow,
		Run, Walk, AirborneNormal, JumpSquatLong, AirborneLong,
		GhostJumpSquatLong, FastAttack, StrongAttack, FallingAttack,
		HorizontalLauncher, VerticalLauncher, GrabHigh, GrabLong,
		PropelHigh, PropelLong, TurnAround, Landing, LandingFallingAttack,
		HuggingWall, WallJumpSquat, HuggingWallLongJumpSquat, AirborneWallJump,
		ReleasePowerAir, ReleasePowerGround, FastAttackAir, JumpSquatSpring,
		IdleTurnAround, WallJumpSquatPlummet, WallJumpPlummet
	};
	IActionState* baseState;
	IActionState* concurrentState;
	
	bool lockBaseState = false;
	bool lockWalk = false;
	bool lockTurning = false;
	bool lockConcurrentState = false;

	//Parent methods
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void setBaseState(ActionStates newState);
	void setConcurrentState(ActionStates newState);
	void updateMovement(float delta, VEC3 deltaMovement);
	void setMovementInput(VEC2 input, float delta);
	void jumpButtonPressed();
	void jumpButtonReleased();
	void longJumpButtonPressed();
	void fastAttackButtonPressed();
	void fastAttackButtonReleased();
	void strongAttackButtonPressed();
	void strongAttackButtonReleased();
	void centerCameraButtonPressed();
	void releasePowerButtonPressed();
	void gainPowerButtonPressed();
	bool isConcurrentActionFree();
	
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

	template <typename T>
	T GetBaseState(TCompPlayerModel::ActionStates state) { return static_cast<T>(baseStates[state]); }
	template <typename T >
	T GetConcurrentState(TCompPlayerModel::ActionStates state) { return static_cast<T>(concurrentStates[state]); }
	
	bool isGrounded = false;
	bool isTouchingCeiling = false;
	bool isAttachedToPlatform = false;
	float maxVerticalSpeed = 0.f;

	float walkingSpeed = 0.f;
	float maxVelocityUpwards = 45.f;

	float huggingWallMinPitch = deg2rad(-25);
	float huggingWallMaxPitch = deg2rad(5);
	float attachWallByInputMinDot = 0.3f;
	float attachWallByFrontMinDot = 0.7f;
	CTimer sameNormalReattachTimer;
	float sameNormalReattachTime = 0.8f;
	PxVec3 lastWallNormal = {0,0,0};

	CHandle grabTarget;
	PxRigidActor* lastWallEntered;

	
private:
	
	CHandle myTransformHandle;
	CHandle colliderHandle;
	CHandle powerGaugeHandle;

	VEC3 respawnPosition;
	VEC3 deltaMovement;
	VEC3 accelerationVector;
	VEC3 velocityVector;
	float baseGravity = 0.f;
	float currentGravity = 0.f;

	PowerStats* ssj1;
	PowerStats* ssj2;
	PowerStats* ssj3;
	PowerStats* currentPowerStats;

	float hp = 8;
	float maxHp = 8;
    int chrysalis = 0;
    int chrysalisTarget = 5;
	bool showVictoryDialog = false;

	CTimer dialogTimer;
	float dialogTime = 15.0f;
 
	PlayerFilterCallback* playerFilterCallback;

	std::map<ActionStates, IActionState*> baseStates;
	std::map<ActionStates, IActionState*> concurrentStates;
	ActionStates nextBaseState;
	ActionStates nextConcurrentState;

	CHandle strongAttackHitbox;
	CHandle fastAttackHitbox;
	CHandle fastAttackAirHitbox;
	CHandle fallingAttackHitbox;
	CHandle fallingAttackLandingHitbox;
	CHandle verticalLauncherHitbox;
	CHandle horizontalLauncherHitbox;
	CHandle grabHitbox;
	CHandle wallJumpPlummetHitbox;
	CHandle releasePowerSmallHitbox;
	CHandle releasePowerBigHitbox;

	PowerStats* loadPowerStats(const json& j);

	void OnGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void OnCollect(const TMsgCollect& msg);
	void OnShapeHit(const TMsgOnShapeHit & msg);
	void OnContact(const TMsgOnContact & msg);
	void OnLevelChange(const TMsgPowerLvlChange& msg);

	void ChangeBaseState(ActionStates newState);
	void ChangeConcurrentState(ActionStates newState);
	void OnDead();
	
	void ApplyGravity(float delta);
	
};
