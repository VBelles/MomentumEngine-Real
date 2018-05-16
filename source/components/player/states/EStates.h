#pragma once


enum State {
	UndefinedState,
	Idle,
	JumpSquat,
	GhostJumpSquat,
	GhostJumpWindow,
	Run,
	Walk,
	AirborneNormal,
	JumpSquatLong,
	AirborneLong,
	GhostJumpSquatLong,
	FastAttack,
	StrongAttack,
	FallingAttack,
	HorizontalLauncher,
	VerticalLauncher,
	GrabHigh,
	GrabLong,
	PropelHigh,
	PropelLong,
	TurnAround,
	Landing,
	LandingFallingAttack,
	HuggingWall,
	WallJumpSquat,
	HuggingWallLongJumpSquat,
	AirborneWallJump,
	ReleasePowerAir,
	ReleasePowerGround,
	FastAttackAir,
	JumpSquatSpring,
	IdleTurnAround,
	WallJumpSquatPlummet,
	WallJumpPlummet,
	Death,
	PitFalling,
	HardKnockbackGround,
	Slide
};

enum ConcurrentState {
	UndefinedConcurrentState,
	Free,
	FastAttack,
	FastAttackAir,
	GrabHigh,
	GrabLong,
	ReleasePowerAir
};