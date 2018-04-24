#pragma once

struct PowerStats {
	float maxHorizontalSpeed = 0.f;
	float rotationSpeed = 0.f;
	float landingLag = 0.f;
	float fallingMultiplier = 1.1f;
	float longGravityMultiplier = 1.f;
	float normalGravityMultiplier = 1.f;
	float currentGravityMultiplier = 1.f;
	float maxVelocityVertical = 30.f;
	float acceleration = 30.f;
	float deceleration = 40.f;
	float airAcceleration = 40.f;
	float shortHopVelocity = 6.f;
	float springJumpVelocity = 23.f;
	float plummetTime = 0.5f;
	VEC3 jumpVelocityVector = { 0.f, 8.f, 0.f };
	VEC3 longJumpVelocityVector = { 0.f, 6.f, 12.f };
	VEC3 wallJumpVelocityVector = { 0.f, 20.f, 7.f };
};