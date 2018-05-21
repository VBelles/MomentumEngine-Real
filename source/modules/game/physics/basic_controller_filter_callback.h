#pragma once

class BasicControllerFilterCallback: public PxControllerFilterCallback {
public:
	bool filter(const PxController& a, const PxController& b) override;
};