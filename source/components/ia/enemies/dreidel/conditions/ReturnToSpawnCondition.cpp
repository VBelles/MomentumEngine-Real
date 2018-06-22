#include "mcv_platform.h"
#include "ReturnToSpawnCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

ReturnToSpawnCondition::ReturnToSpawnCondition(Dreidel* dreidel): dreidel(dreidel) {
}

bool ReturnToSpawnCondition::testCondition(float delta) {
	return false;
}
