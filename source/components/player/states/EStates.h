#pragma once

#define STATES \
	etype(UndefinedState), \
	etype(Death), \
	etype(PitFalling), \
	etype(HardKnockbackGround), \
	etype(HardKnockbackAir), \
	etype(Slide), \
	etype(LandingHurt), \
	etype(PropelHigh), \
	etype(PropelLong), \
	etype(SpendCoins), \
	etype(GhostJumpSquat), \
	etype(GhostJumpSquatLong), \
	etype(GhostJumpWindow), \
	etype(SoftLanding), \
	etype(HardLanding), \
	etype(Dodge), \
	etype(AirDodge), \
	etype(ReleasePowerGround), \
	etype(JumpSquat), \
	etype(JumpSquatLong), \
	etype(JumpSquatSpring), \
	etype(AirborneNormal), \
	etype(AirborneLong), \
	etype(SpringJump), \
	etype(HuggingWall), \
	etype(WallJumpSquat), \
	etype(HuggingWallLongJumpSquat), \
	etype(AirborneWallJump), \
	etype(WallJumpSquatPlummet), \
	etype(WallJumpPlummet), \
	etype(FallingAttack), \
	etype(HorizontalLauncher), \
	etype(VerticalLauncher), \
	etype(StrongAttack), \
	etype(StrongAttack2), \
	etype(StrongAttack3), \
	etype(StrongFinisher1), \
	etype(StrongFinisher2), \
	etype(FastFinisher1), \
	etype(FastFinisher2), \
	etype(TurnAround), \
	etype(Run), \
	etype(Walk), \
	etype(IdleTurnAround), \
	etype(Idle)

#define etype(x) x
typedef enum { STATES } State;
#undef etype
#define etype(x) #x
static const char *strStates[] = { STATES };
#undef etype

#define etype(x) { #x, x }
static const std::map<std::string, State> statesMap = { STATES };
#undef etype

#define CONCURRENT_STATES \
	etype(UndefinedConcurrentState), \
	etype(Free), \
	etype(ReleasePowerAir), \
	etype(GrabHigh), \
	etype(GrabLong), \
	etype(FastAttack), \
	etype(FastAttack2), \
	etype(FastAttackAir), \
	etype(SoftKnockbackGround), \
	etype(SoftKnockbackAir)

#define etype(x) x
typedef enum { CONCURRENT_STATES } ConcurrentState;
#undef etype
#define etype(x) #x
static const char *strConcurrentStates[] = { CONCURRENT_STATES };

#define etype(x) { #x, x }
static const std::map<std::string, ConcurrentState> concurrentStatesMap = { CONCURRENT_STATES };
#undef etype

namespace States {
	const char* toString(State state);
	const char* toString(ConcurrentState state);

	State getState(std::string state);
	ConcurrentState getConcurrentState(std::string state);
}

