#pragma once

#include "components/comp_base.h"
#include "power_stats.h"
#include "entity/common_msgs.h"
#include "components/player/attack_info.h"

class IActionState;
class TCompCollider;
class TCompTransform;
class TCompPowerGauge;
class TCompCollectableManager;
class StateManager;
struct TMsgRespawnChanged;

struct HitState {
	CHandle entity;
	PxControllerShapeHit hit;
	float dotUp = 0.f;

	HitState(CHandle entity, PxControllerShapeHit hit) :
		entity(entity), hit(hit), dotUp(hit.worldNormal.dot(PxVec3(0, 1, 0))) {
	}
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
	CHandle collectableManagerHandle;

	VEC3 respawnPosition;
	VEC3 deltaMovement;
	VEC3 accelerationVector;
	VEC3 velocityVector;
	float baseGravity = 0.f;
	float currentGravity = 0.f;

	MoveState moveState;

	std::string materials[NUMBER_OF_POWER_LEVELS];
	PowerStats* powerStats[NUMBER_OF_POWER_LEVELS];
	PowerStats* currentPowerStats = nullptr;

	const float PLAYER_MAX_HP = 8.f;
	float maxHp = 0.f;
	float hp = 0.f;

	bool isInvulnerable = false;
	CTimer invulnerableTimer;
	float invulnerableTime = 1.f;

	//TODO Esto aqu� es criminal, milestone 1 ftw 
	int chrysalisTarget = 5;
	bool showVictoryDialog = false;
	CTimer dialogTimer;
	float dialogTime = 15.0f;

	StateManager* stateManager = nullptr;

	AttackInfo receivedAttack;

	//Messages
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onShapeHit(const TMsgShapeHit & msg);
	void onControllerHit(const TMsgControllerHit & msg);
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
	bool lockAttack = false;
	bool wannaJump = false;
	bool isWalking = false;

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
	void spendCoinsButtonPressed();
	void spendCoinsButtonReleased();
	void dodgeButtonPressed();
	void walkButtonPressed();
	void gainPowerButtonPressed();

	TCompTransform* getTransform();
	TCompCollider* getCollider();
	PxCapsuleController* getController();
	TCompPowerGauge* getPowerGauge();
	TCompCollectableManager* getCollectableManager();

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
	void resetHp() { maxHp = PLAYER_MAX_HP; setHp(maxHp); }
	float getHp() { return hp; }
	void setHp(float hp);
	void setRespawnPosition(VEC3 position);
	VEC3 getRespawnPosition() { return respawnPosition; }
	void disableOutline();
	void enableOutline();

	StateManager* getStateManager() { return stateManager; }
};