#include "mcv_platform.h"
#include "FalseCondition.h"

REGISTER_BTCONDITION("FalseCondition", FalseCondition);

FalseCondition::FalseCondition() {
	type = "FalseCondition";
}

bool FalseCondition::testCondition(float delta) {
	return false;
}
