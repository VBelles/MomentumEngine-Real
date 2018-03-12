#include "mcv_platform.h"
#include "PowerGauge.h"
#include "utils/utils.h"
  
void PowerGauge::Update(float delta) {
	IncreasePower(-dropSpeed * delta);

	int lastPowerLevel = powerLevel;
	powerLevel = power < powerPerLevel ? 1 : power < powerPerLevel * 2 ? 2 : 3;
	if (lastPowerLevel != powerLevel) powerListener->OnLevelChange(powerLevel);
}

void PowerGauge::ReleasePower() {
	IncreasePower(-powerPerLevel);
}

void PowerGauge::GainPower() {
	IncreasePower(powerPerLevel);
}

void PowerGauge::IncreasePower(float increment) {
	power += increment;
	power = clamp(power, 0.f, maxPower);
}

void PowerGauge::ResetPower() {
	IncreasePower(-maxPower);
}
