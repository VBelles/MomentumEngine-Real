#pragma once

class TCompPlayerModel;
class TCompTransform;
class TCompCollider;
class TCompRender;
class TCompCamera;
class TCompSkeleton;
class TCompHitboxes;
class TCompRenderBlurRadial;
class IActionState;

class StateManager {
public:
	enum ActionState {
		Idle, JumpSquat, GhostJumpSquat, GhostJumpWindow,
		Run, Walk, AirborneNormal, JumpSquatLong, AirborneLong,
		GhostJumpSquatLong, StrongAttack, FallingAttack,
		HorizontalLauncher, VerticalLauncher,
		PropelHigh, PropelLong, TurnAround, Landing, LandingFallingAttack,
		HuggingWall, WallJumpSquat, HuggingWallLongJumpSquat, AirborneWallJump,
		ReleasePowerGround, JumpSquatSpring,
		IdleTurnAround, WallJumpSquatPlummet, WallJumpPlummet, Death, PitFalling,
		HardKnockbackGround, Slide
	};

	enum ConcurrentActionState {
		Free, FastAttack, FastAttackAir, GrabHigh, GrabLong, ReleasePowerAir
	};
private:
	CHandle entityHandle;
	CHandle playerModelHandle;
	CHandle transformHandle;
	CHandle colliderHandle;
	CHandle renderHandle;
	CHandle cameraHandle;
	CHandle skeletonHandle;
	CHandle hitboxesHandle;
	CHandle renderBlurRadialHandle;

	std::map<ActionState, IActionState*> states;
	std::map<ConcurrentActionState, IActionState*> concurrentStates;

	IActionState* baseState;
	IActionState* concurrentState;
	IActionState* nextBaseState;
	IActionState* nextConcurrentState;

	template <typename T>
	void registerState(ActionState state, T);
	template <typename T>
	void registerState(ConcurrentActionState state, T);
	
public:
	
	StateManager(CHandle entityHandle);
	~StateManager();

	void registerStates();

	void updateStates(float delta);

	void changeState(ActionState newState);

	void changeConcurrentState(ConcurrentActionState newState);

	void performStateChange();

	CEntity* getEntity();
	TCompPlayerModel* getPlayerModel();
	TCompTransform* getTransform();
	TCompCollider* getCollider();
	TCompRender* getRender();
	TCompCamera* getCamera();
	TCompSkeleton* getSkeleton();
	TCompHitboxes* getHitboxes();
	TCompRenderBlurRadial* getBlurRadial();

};


