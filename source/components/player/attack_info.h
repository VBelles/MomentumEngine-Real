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
		float duration;
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

	AttackInfo();
	AttackInfo(const AttackInfo& other);
	AttackInfo& operator=(const AttackInfo& other);
	void load(const json& j);
	~AttackInfo();
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
