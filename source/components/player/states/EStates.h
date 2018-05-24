#pragma once

#define STATES \
	etype(UndefinedState), \
	etype(Idle), \
	etype(JumpSquat), \
	etype(GhostJumpSquat), \
	etype(GhostJumpWindow), \
	etype(Run), \
	etype(Walk), \
	etype(AirborneNormal), \
	etype(JumpSquatLong), \
	etype(AirborneLong), \
	etype(GhostJumpSquatLong), \
	etype(StrongAttack), \
	etype(StrongAttack2), \
	etype(StrongAttack3), \
	etype(StrongFinisher1), \
	etype(StrongFinisher2), \
	etype(FastFinisher1), \
	etype(FastFinisher2), \
	etype(FallingAttack), \
	etype(HorizontalLauncher), \
	etype(VerticalLauncher), \
	etype(PropelHigh), \
	etype(PropelLong), \
	etype(TurnAround), \
	etype(Landing), \
	etype(LandingFallingAttack), \
	etype(HuggingWall), \
	etype(WallJumpSquat), \
	etype(HuggingWallLongJumpSquat), \
	etype(AirborneWallJump), \
	etype(ReleasePowerGround), \
	etype(JumpSquatSpring), \
	etype(IdleTurnAround), \
	etype(WallJumpSquatPlummet), \
	etype(WallJumpPlummet), \
	etype(Death), \
	etype(PitFalling), \
	etype(HardKnockbackGround), \
	etype(HardKnockbackAir), \
	etype(Slide), \
	etype(SpringJump), \
	etype(Dodge)
#define etype(x) x
typedef enum { STATES } State;
#undef etype
#define etype(x) #x
static const char *strStates[] = { STATES };
#undef etype

#define CONCURRENT_STATES \
	etype(UndefinedConcurrentState), \
	etype(Free), \
	etype(FastAttack), \
	etype(FastAttack2), \
	etype(FastAttackAir), \
	etype(GrabHigh), \
	etype(GrabLong), \
	etype(ReleasePowerAir), \
	etype(SoftKnockbackAir), \
	etype(SoftKnockbackGround)

#define etype(x) x
typedef enum { CONCURRENT_STATES } ConcurrentState;
#undef etype
#define etype(x) #x
static const char *strConcurrentStates[] = { CONCURRENT_STATES };


namespace States {
	const char* toString(State state);
	const char* toString(ConcurrentState state);
}

