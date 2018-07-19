#include "mcv_platform.h"
#include "FalseCondition.h"

REGISTER_BTCONDITION("FalseCondition", FalseCondition);

bool FalseCondition::testCondition(float delta) {
	return false;
}