#ifndef INC_COMMON_ENTITY_MSGS_
#define INC_COMMON_ENTITY_MSGS_

#include "msgs.h"
#include "geometry/transform.h"

struct TMsgEntityCreated {
	DECL_MSG_ID();
};

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
	bool givesPower = false;
	bool activatesMechanism = false;
	Stun* stun = nullptr;
	VerticalLauncher* verticalLauncher = nullptr;
	HorizontalLauncher* horizontalLauncher = nullptr;
	Grab* grab = nullptr;
	Propel* propel = nullptr;

	void copy(const AttackInfo &toCopy) {
		damage = toCopy.damage;
		givesPower = toCopy.givesPower;
		activatesMechanism = toCopy.activatesMechanism;
		release();
		if (toCopy.stun) {
			stun = new Stun{ toCopy.stun->duration };
		}
		if (toCopy.verticalLauncher) {
			dbg("attack info copy vertical launcher duration: %f\n", toCopy.verticalLauncher->suspensionDuration);
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

// Sent to all entities from a parsed file once all the entities
// in that file have been created. Used to link entities between them
struct TEntityParseContext;
struct TMsgEntitiesGroupCreated {
	const TEntityParseContext& ctx;
	DECL_MSG_ID();
};

struct TMsgAllScenesCreated {
	DECL_MSG_ID();
};

struct TMsgLockCameraInput {
	DECL_MSG_ID();
	bool isLocked;
};

struct TMsgMechanismActivated {
	DECL_MSG_ID();
};


struct TMsgMechanismDeactivated {
	DECL_MSG_ID();
};

struct TMsgMechanismSystemActivated {
	DECL_MSG_ID();
};

struct TMsgAssignBulletOwner {
	CHandle h_owner;
	DECL_MSG_ID();
};

struct TMsgAssignRangedAttackOwner {
	CHandle ownerHandle;
	AttackInfo attackInfo;
	VEC3 initialPos;
	VEC3 direction;
	DECL_MSG_ID();
};

struct TMsgDamage {
	CHandle h_sender;
	CHandle h_bullet;
	int damage;
	DECL_MSG_ID();
};


struct TMsgDestroy {
	CHandle h_sender;
	DECL_MSG_ID();
};

struct TMsgHitboxEnter {
	CHandle h_other_entity;
	DECL_MSG_ID();
};

struct TMsgAttackHit {
	CHandle attacker;
	AttackInfo info;
	DECL_MSG_ID();
};

struct TMsgGetPower {
	CHandle sender;
	float power;
	DECL_MSG_ID();
};

struct TMsgGainPower {
	CHandle sender;
	float power;
	DECL_MSG_ID();
};

struct TMsgPowerLvlChange {
	CHandle sender;
	int powerLvl;
	DECL_MSG_ID();
};

struct TMsgPurityChange {
	CHandle sender;
	bool isPure;
	DECL_MSG_ID();
};

struct TMsgOutOfBounds {
	CHandle sender;
	DECL_MSG_ID();
};

struct TMsgColliderDestroyed {
	DECL_MSG_ID();
};

struct TMsgRespawn {
	DECL_MSG_ID();
};

struct TMsgDefineLocalAABB {
	AABB* aabb;
	DECL_MSG_ID();
};

struct TMsgOnShapeHit {
	const physx::PxControllerShapeHit& hit;
	DECL_MSG_ID();
};

struct TMsgOnContact {
	CHandle otherEntity;
	physx::PxContactPair pair;
	DECL_MSG_ID();
};



#endif
