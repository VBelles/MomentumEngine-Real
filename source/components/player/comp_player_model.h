#pragma once

#include "power_stats.h"
#include "entity/common_msgs.h"
#include "components/player/attack_info.h"
#include "components/comp_collectable.h"
#include "components/comp_hitboxes.h"
#include "modules/system_modules/sound/comp_sound.h"
#include "modules/system_modules/sound/music_player.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/comp_power_gauge.h"
#include "components/player/comp_collectable_manager.h"
#include "components/controllers/comp_camera_player.h"
#include "components/player/states/StateManager.h"
#include "modules/system_modules/particles/comp_particles.h"

class IActionState;
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
	std::vector<HitState> allHits;

	PxShape* standingShape = nullptr;
};

class TCompPlayerModel : public TCompBase {
private:
	CHandle entityHandle;
	CHandle transformHandle;
	CHandle colliderHandle;
	CHandle renderHandle;
	CHandle skeletonHandle;
	CHandle soundHandle;
	CHandle powerGaugeHandle;
	CHandle collectableManagerHandle;
	CHandle cameraRenderHandle;
	CHandle cameraPlayerHandle;
	CHandle hitboxesHandle;
	CHandle particlesHandle;

	VEC3 respawnPosition;
	float respawnYaw;
	VEC3 deltaMovement;
	VEC3 accelerationVector;
	VEC3 velocityVector;
	float baseGravity = 0.f;
	float currentGravity = 0.f;
	std::vector<std::string> initialLockedStates;
	std::vector<std::string> initialLockedConcurrentStates;

	MoveState moveState;

	std::string particleSystems[NUMBER_OF_POWER_LEVELS];
	std::string materials[NUMBER_OF_POWER_LEVELS];
	PowerStats* powerStats[NUMBER_OF_POWER_LEVELS];
	PowerStats* currentPowerStats = nullptr;

	const float PLAYER_MAX_HP = 5.f;
	float maxHp = 0.f;
	float hp = 0.f;

	float maxAttackSlots = 4.f;
	float attackSlotsTaken = 0.f;
	std::set<std::string> attackers;

	bool isInvulnerable = false;
	CTimer2 invulnerableTimer;
	float invulnerableTime = 1.f;

	CTimer damageVisionTimer;
	float damageVisionTime;
	float damageVisionDefaultTime = 2.f;
	float damageVisionPlummetTimePercentage = 0.1f;
	float damageVisionPlummetTime;
	float damageVisionStayTimePercentage = 0.7f;
	float damageVisionStayTime;
	float damageVisionOriginalSaturationLevel;
	float damageVisionLowestSaturationLevel = 0.35f;

	//TODO Esto aqu� es criminal, milestone 1 ftw 
	int chrysalisTarget = 5;
	bool showVictoryDialog = false;
	CTimer dialogTimer;
	float dialogTime = 15.0f;

	StateManager* stateManager = nullptr;

	AttackInfo receivedAttack;

	//Messages
	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
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

	//para poder rotar desde lua
	bool isPlayerRotating = false;
	VEC3 rotatingTargetPos;
	float rotationSpeed;

public:
	DECL_SIBLING_ACCESS();
	~TCompPlayerModel();

	bool isOnPlatform = false;

	bool lockWalk = false;
	bool lockTurning = false;
	bool lockAttack = false;
	bool wannaJump = false;
	bool isWalking = false;

	bool lockFallingAttack = false;
	bool lockAirDodge = false;
	
	float maxVerticalSpeed = 0.f;

	float walkingSpeed = 0.f;
	float maxVelocityUpwards = 45.f;

	float huggingWallMinPitch = deg2rad(-25);
	float huggingWallMaxPitch = deg2rad(15);
	float attachWallByInputMinDot = 0.3f;
	float attachWallByFrontMinDot = 0.7f;
	CTimer sameNormalReattachTimer;
	float sameNormalReattachTime = 0.8f;
	CTimer lockHuggingWallTimer;
	float lockHuggingWallTime = 0.8f;
	PxVec3 lastWallNormal = { 0,0,0 };

	CHandle grabTarget;

	CTimer disabledClimbingTimer;
	bool canClimb = true;
	CTimer platformChangeSlopeTimer;

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

	CEntity* getPlayerEntity();
	TCompTransform* getTransform();
	TCompCollider* getCollider();
	PxCapsuleController* getController();
	TCompRender* getRender();
	TCompHitboxes* getHitboxes();
	TCompCamera* getCameraRender();
	TCompCameraPlayer* getCameraPlayer();
	TCompSkeleton* getSkeleton();
	TCompPowerGauge* getPowerGauge();
	TCompSound* getSound();
	TCompParticles* getParticles();
	TCompCollectableManager* getCollectableManager();
	CMusicPlayer* getMusicPlayer();

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
	void startDamageVision(float time);
	void resetHp() { maxHp = PLAYER_MAX_HP; setHp(maxHp); }
	float getHp() { return hp; }
	float getMaxHp() { return maxHp; }
	void setHp(float hp);
	void setMaxHp(float hp);
	void setRespawnPosition(VEC3 position, float yaw = 0.f);
	VEC3 getRespawnPosition() { return respawnPosition; }
	float getRespawnYaw() { return respawnYaw; }
	void disableOutline();
	void enableOutline();
	void stopPlayerVelocity();
	void rotatePlayerTowards(VEC3 targetPos, float rotationSpeed);
	void walkTo(VEC3 targetPosition);
	void disableClimbing();
	void onPlatform();

	StateManager* getStateManager() { return stateManager; }

	bool isGrounded();

	int getNumberOfCoins();
	int getMaxNumberOfCoins();
	float getPowerPerCoin();

	bool addAttacker(std::string attacker, float slots);
	void removeAttacker(std::string attacker, float slots);

	void lockState(std::string state);
	void lockConcurrentState(std::string state);
	void unlockState(std::string state);
	void changeState(std::string state);
	void changeConcurrentState(std::string state);
	void setDummyState(std::string animation, bool isLoop, float duration, std::string exitAnimation);

	void teleport(VEC3 targetPos);
};
