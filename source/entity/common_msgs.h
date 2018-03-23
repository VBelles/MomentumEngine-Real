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

	~AttackInfo() {
		delete stun;
		delete verticalLauncher;
		delete horizontalLauncher;
		delete grab;
		delete propel;
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
	const TEntityParseContext& ctx;
	DECL_MSG_ID();
};

struct TMsgAssignBulletOwner {
    CHandle h_owner;
    DECL_MSG_ID();
};

struct TMsgDamage {
    CHandle h_sender;
    CHandle h_bullet;
    int damage;
    DECL_MSG_ID();
};

struct TMsgCollect {
    std::string type;
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
	physx::PxControllerShapeHit hit;
	DECL_MSG_ID();
};



#endif
