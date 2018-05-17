#pragma once

#include "components/comp_base.h"
#include "power_stats.h"
#include "entity/common_msgs.h"
#include "modules/game/physics/basic_controller_hit_callback.h"

class IActionState;
class TCompCamera;
class TCompCollider;
class TCompTransform;
class TCompPowerGauge;
class TCompSkeleton;
class PlayerFilterCallback;
class StateManager;

struct TMsgRespawnChanged;
struct TMsgCollect;


struct HitState {
	CHandle entity;
	PxControllerShapeHit hit;
	float dotUp = 0.f;
};

struct MoveState {
	bool isTouchingTop = false;
	bool isTouchingBot = false;
	bool isTouchingSide = false;

	std::vector<HitState> topHits;
	std::vector<HitState> botHits;
	std::vector<HitState> sideHits;
};

class TCompPlayerModel : public TCompBase {
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

	MoveState moveState;

	std::string materials[3];
	PowerStats* powerStats[3];
	PowerStats* currentPowerStats;

	const float PLAYER_MAX_HP = 8.f;
	float maxHp = PLAYER_MAX_HP;
	float hp = PLAYER_MAX_HP;

	bool isInvulnerable = false;
	CTimer invulnerableTimer;
	float invulnerableTime = 1.f;

	int coins = 0;

	//TODO Esto aqu� es criminal, milestone 1 ftw 
	int chrysalis = 0;
	int chrysalisTarget = 5;
	bool showVictoryDialog = false;
	CTimer dialogTimer;
	float dialogTime = 15.0f;
	

	PlayerFilterCallback* playerFilterCallback = nullptr;

	StateManager* stateManager = nullptr;

	AttackInfo receivedAttack;

	//Messages
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onCollect(const TMsgCollect& msg);
	void onShapeHit(const TMsgShapeHit & msg);
	void onLevelChange(const TMsgPowerLvlChange& msg);
	void onAttackHit(const TMsgAttackHit& msg);
	void onHitboxEnter(const TMsgHitboxEnter& msg);
	void onGainPower(const TMsgGainPower& msg);
	void onOutOfBounds(const TMsgOutOfBounds& msg);
	void onRespawnChanged(const TMsgRespawnChanged& msg);
	void onPurityChange(const TMsgPurityChange& msg);

	PowerStats* loadPowerStats(const json& j);

	void applyGravity(float delta);

public:
	DECL_SIBLING_ACCESS();
	~TCompPlayerModel();

	bool lockWalk = false;
	bool lockTurning = false;
	bool wannaJump = false;

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

	//Parent methods
	static void registerMsgs();
	void debugInMenu();
	void debugInMenu(PowerStats * powerStats, std::string name);
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

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
	AttackInfo* getReceivedAttack() { return &receivedAttack; }

	void damage(float damage);
	void makeInvulnerable(float time);
	void resetHp() { hp = maxHp; }
	float getHp() { return hp; }
	void setHp(float hp);
	void setRespawnPosition(VEC3 position);
	VEC3 getRespawnPosition() { return respawnPosition; }
	void disableOutline();
	void enableOutline();

	StateManager* getStateManager() { return stateManager; }
};