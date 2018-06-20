#pragma once

class BasicControllerBehavior : public PxControllerBehaviorCallback {
private:
	CHandle entityHandle;

public:
	BasicControllerBehavior(CHandle entityHandle);

	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor) override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller) override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle) override;

	virtual ~BasicControllerBehavior() override {};
};
