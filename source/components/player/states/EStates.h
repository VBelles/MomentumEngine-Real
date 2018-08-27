#pragma once

#define STATES \
	etype(UndefinedState), \
	etype(Death), \
	etype(PitFalling), \
	etype(HardKnockbackGround), \
	etype(HardKnockbackAir), \
	etype(LandingHurt), \
	etype(Slide), \
	etype(SpendCoins), \
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
	etype(SoftLanding), \
	etype(HardLanding), \
	etype(HuggingWall), \
	etype(WallJumpSquat), \
	etype(HuggingWallLongJumpSquat), \
	etype(AirborneWallJump), \
	etype(ReleasePowerGround), \
	etype(JumpSquatSpring), \
	etype(WallJumpSquatPlummet), \
	etype(WallJumpPlummet), \
	etype(SpringJump), \
	etype(Dodge), \
	etype(AirDodge), \
	etype(IdleTurnAround)

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

#define etype(x) { #x, x }
static const std::map<std::string, ConcurrentState> concurrentStatesMap = { CONCURRENT_STATES };
#undef etype

namespace States {
	const char* toString(State state);
	const char* toString(ConcurrentState state);

	State getState(std::string state);
	ConcurrentState getConcurrentState(std::string state);
}

