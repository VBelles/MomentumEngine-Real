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
	StrongAttack,
	FallingAttack,
	HorizontalLauncher,
	VerticalLauncher,
	PropelHigh,
	PropelLong,
	TurnAround,
	Landing,
	LandingFallingAttack,
	HuggingWall,
	WallJumpSquat,
	HuggingWallLongJumpSquat,
	AirborneWallJump,
	ReleasePowerGround,
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