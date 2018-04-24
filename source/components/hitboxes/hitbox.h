#pragma once
class Hitbox {
private:

public:
	struct HitboxConfig {
		PxGeometryType::Enum shapeType;
		VEC3 offset;
		VEC3 halfExtent;
		float radius;
		float height;
		unsigned int group;
		unsigned int mask;
	};
	PxRigidActor* actor;
	Hitbox();
	~Hitbox();
};

