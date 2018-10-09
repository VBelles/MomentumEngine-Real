#pragma once

#include "msgs.h"

struct TMsgEntityCreated {
	DECL_MSG_ID();
};

struct TMsgEntityDestroyed {
    DECL_MSG_ID();
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

struct TMsgMechanismSystemDeactivated {
	DECL_MSG_ID();
};

struct TMsgPerfectDodged {
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

struct TMsgHeal {
	CHandle h_sender;
	int heal;
	DECL_MSG_ID();
};

struct TMsgDestroy {
	CHandle h_sender;
	DECL_MSG_ID();
};

struct TMsgHitboxEnter {
	std::string hitbox;
	CHandle h_other_entity;
	DECL_MSG_ID();
};

struct TMsgGetPower {
	CHandle sender;
	float power;
	DECL_MSG_ID();
};

struct TMsgSetVisible {
  bool visible;
  DECL_MSG_ID();
};

struct TMsgGainPower {
	CHandle sender;
	float power;
	DECL_MSG_ID();
};

struct TMsgLosePower {
	CHandle attacker;
	float power;
	DECL_MSG_ID();
};

struct TMsgPowerLvlChange {
	CHandle sender;
	int powerLvl;
	int prevPowerLvl;
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

struct TMsgRespawn {
	DECL_MSG_ID();
};

struct TMsgDefineLocalAABB {
	AABB* aabb;
	DECL_MSG_ID();
};

struct TMsgOnContact {
	CHandle otherEntity;
	physx::PxContactPair pair;
	DECL_MSG_ID();
};
