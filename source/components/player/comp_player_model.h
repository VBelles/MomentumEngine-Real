#pragma once

#include "components/comp_base.h"
#include "power_stats.h"

class IActionState;
class TCompCamera;
class TCompCollider;
class TCompTransform;
class TCompPowerGauge;
class TCompSkeleton;
class PlayerFilterCallback;

struct TMsgEntitiesGroupCreated;
struct TMsgCollect;
struct TMsgOnShapeHit;
struct TMsgPowerLvlChange;
struct TMsgAttackHit;
struct TMsgHitboxEnter;
struct TMsgGainPower;
struct TMsgGainPower;
struct TMsgOutOfBounds;


class TCompPlayerModel : public TCompBase {
public:
	enum ActionStates {
		Idle, JumpSquat, GhostJumpSquat, GhostJumpWindow,
		Run, Walk, AirborneNormal, JumpSquatLong, AirborneLong,
		GhostJumpSquatLong, FastAttack, StrongAttack, FallingAttack,
		HorizontalLauncher, VerticalLauncher, GrabHigh, GrabLong,
		PropelHigh, PropelLong, TurnAround, Landing, LandingFallingAttack,
		HuggingWall, WallJumpSquat, HuggingWallLongJumpSquat, AirborneWallJump,
		ReleasePowerAir, ReleasePowerGround, FastAttackAir, JumpSquatSpring,
		IdleTurnAround, WallJumpSquatPlummet, WallJumpPlummet, Death, PitFalling,
		HardKnockbackGround
	};

private:
	CHandle transformHandle;
	CHandle colliderHandle;
	CHandle powerGaugeHandle;
	CHandle skeletonHandle;

	VEC3 respawnPosition;
	VEC3 deltaMovement;
	VEC3 accelerationVector;
	VEC3 velocityVector;
	float baseGravity = 0.f;
	float currentGravity = 0.f;

	std::string materials[3];
	PowerStats* powerStats[3];
	PowerStats* currentPowerStats;

	float hp = 8;
	float maxHp = 8;

	bool isInvulnerable = false;
	CTimer invulnerableTimer;
	float invulnerableTime = 1.f;

	//TODO Esto aquí es criminal, milestone 1 ftw 
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

	//Messages
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onCollect(const TMsgCollect& msg);
	void onShapeHit(const TMsgOnShapeHit & msg);
	void onLevelChange(const TMsgPowerLvlChange& msg);
	void onAttackHit(const TMsgAttackHit& msg);
	void onHitboxEnter(const TMsgHitboxEnter& msg);
	void onGainPower(const TMsgGainPower& msg);
	void onOutOfBounds(const TMsgOutOfBounds& msg);

	PowerStats* loadPowerStats(const json& j);
	void changeBaseState(ActionStates newState);
	void changeConcurrentState(ActionStates newState);

	void applyGravity(float delta);

public:
	DECL_SIBLING_ACCESS();
	~TCompPlayerModel();
	IActionState* baseState;
	IActionState* concurrentState;

	bool lockBaseState = false;
	bool lockWalk = false;
	bool lockTurning = false;
	bool lockConcurrentState = false;

	bool wannaJump = false;
	bool isGrounded = false;
	bool isTouchingCeiling = false;
	bool isAttachedToPlatform = false;
	float maxVerticalSpeed = 0.f;

	float walkingSpeed = 0.f;
	float maxVelocityUpwards = 45.f;

	float huggingWallMinPitch = deg2rad(-30);
	float huggingWallMaxPitch = deg2rad(10);
	float attachWallByInputMinDot = 0.3f;
	float attachWallByFrontMinDot = 0.7f;
	CTimer sameNormalReattachTimer;
	float sameNormalReattachTime = 0.8f;
	PxVec3 lastWallNormal = { 0,0,0 };

	CHandle grabTarget;
	PxRigidActor* lastWallEntered;

	//Parent methods
	static void registerMsgs();
	void debugInMenu();
	void debugInMenu(PowerStats * powerStats, std::string name);
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void setBaseState(ActionStates newState);
	void setConcurrentState(ActionStates newState);
	void updateMovement(float delta, VEC3 deltaMovement);
	void sweep(VEC3 deltaMovement);
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

	TCompTransform* getTransform();
	TCompCollider* getCollider();
	PxCapsuleController* getController();
	TCompCamera* getCamera();
	TCompPowerGauge* getPowerGauge();
	TCompSkeleton* getSkeleton();

	VEC3* getAccelerationVector() { return &accelerationVector; }
	VEC3* getVelocityVector() { return &velocityVector; }
	float getGravity() { return currentGravity; }
	void setGravity(float newGravity) { currentGravity = newGravity; }
	void setGravityMultiplier(float multiplier) { currentGravity = baseGravity * multiplier; }
	void resetGravity() { currentGravity = baseGravity; }

	PowerStats* getPowerStats() { return currentPowerStats; }

	template <typename T>
	T getBaseState(TCompPlayerModel::ActionStates state) { return static_cast<T>(baseStates[state]); }
	template <typename T >
	T getConcurrentState(TCompPlayerModel::ActionStates state) { return static_cast<T>(concurrentStates[state]); }

	void damage(float damage);
	void resetHp() { hp = maxHp; }
	float getHp() { return hp; }
	void setHp(float hp);
	void setRespawnPosition(VEC3 position);
	VEC3 getRespawnPosition() { return respawnPosition; }
};