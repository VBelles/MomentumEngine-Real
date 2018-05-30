#pragma once

class BasicControllerBehavior : public PxControllerBehaviorCallback {
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor) override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller) override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle) override;
};
