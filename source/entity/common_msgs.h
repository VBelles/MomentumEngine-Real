#ifndef INC_COMMON_ENTITY_MSGS_
#define INC_COMMON_ENTITY_MSGS_

#include "msgs.h"
#include "geometry/transform.h"

struct TMsgEntityCreated {
    DECL_MSG_ID();
};

// Sent to all entities from a parsed file once all the entities
// in that file have been created. Used to link entities between them
struct TEntityParseContext;
struct TMsgEntitiesGroupCreated {
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
    float damage;
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
	float damage;
	DECL_MSG_ID();
};

struct TMsgGrabbed {
	CHandle attacker;
	DECL_MSG_ID();
};

struct TMsgPropelled {
	CHandle attacker;
	VEC3 velocityVector;
	DECL_MSG_ID();
};

struct TMsgLaunchedVertically {
	CHandle attacker;
	float damage;
	DECL_MSG_ID();
};

struct TMsgLaunchedHorizontally {
	CHandle attacker;
	float damage;
	VEC3 direction;
	DECL_MSG_ID();
};

struct TMsgGainPower {
	CHandle sender;
	float power;
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

/*
struct TMsgTimeOut {
  DECL_MSG_ID();
};

struct TMsgSpawn {
  DECL_MSG_ID();
};

struct TMsgDefineLocalAABB {
  AABB* aabb;
  DECL_MSG_ID();
};

struct TMsgSpawnAt {
  CTransform where;
  DECL_MSG_ID();
};
*/

#endif
