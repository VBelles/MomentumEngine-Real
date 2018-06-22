#include "mcv_platform.h"
#include "OnStunCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

OnStunCondition::OnStunCondition(Dreidel* dreidle): dreidel(dreidel) {
}

bool OnStunCondition::testCondition(float delta) {
	return false;
}
