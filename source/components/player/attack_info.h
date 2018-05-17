#pragma once
#include "entity/msgs.h"

struct AttackInfo {
	struct Stun {
		float duration;
	};
	struct VerticalLauncher {
		float suspensionDuration;
		VEC3 velocity;
	};
	struct HorizontalLauncher {
		float suspensionDuration;
		VEC3 velocity;
	};
	struct Grab {
		float duration;
	};
	struct Propel {
		VEC3 velocity;
	};
	float damage = 0.f;
	float invulnerabilityTime = 0.f;
	bool givesPower = false;
	bool activatesMechanism = false;
	Stun* stun = nullptr;
	VerticalLauncher* verticalLauncher = nullptr;
	HorizontalLauncher* horizontalLauncher = nullptr;
	Grab* grab = nullptr;
	Propel* propel = nullptr;

	AttackInfo& AttackInfo::operator=(const AttackInfo& toCopy) {
		damage = toCopy.damage;
		invulnerabilityTime = toCopy.invulnerabilityTime;
		givesPower = toCopy.givesPower;
		activatesMechanism = toCopy.activatesMechanism;
		release();
		if (toCopy.stun) {
			stun = new Stun{ toCopy.stun->duration };
		}
		if (toCopy.verticalLauncher) {
			verticalLauncher = new VerticalLauncher{ toCopy.verticalLauncher->suspensionDuration, toCopy.verticalLauncher->velocity };
		}
		if (toCopy.horizontalLauncher) {
			horizontalLauncher = new HorizontalLauncher{ toCopy.horizontalLauncher->suspensionDuration, toCopy.horizontalLauncher->velocity };
		}
		if (toCopy.grab) {
			grab = new Grab{ toCopy.grab->duration };
		}
		if (toCopy.propel) {
			propel = new Propel{ toCopy.propel->velocity };
		}
		return *this;
	}

	void release() {
		if (stun) {
			delete stun;
			stun = nullptr;
		}
		if (verticalLauncher) {
			delete verticalLauncher;
			verticalLauncher = nullptr;
		}
		if (horizontalLauncher) {
			delete horizontalLauncher;
			horizontalLauncher = nullptr;
		}
		if (grab) {
			delete grab;
			grab = nullptr;
		}
		if (propel) {
			delete propel;
			propel = nullptr;
		}
	}

	~AttackInfo() {
		release();
	}
};

struct TMsgAssignRangedAttackOwner {
	CHandle ownerHandle;
	AttackInfo attackInfo;
	VEC3 initialPos;
	VEC3 direction;
	DECL_MSG_ID();
};

struct TMsgAttackHit {
	CHandle attacker;
	AttackInfo info;
	DECL_MSG_ID();
};

