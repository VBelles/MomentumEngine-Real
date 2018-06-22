#include "mcv_platform.h"
#include "StepBackAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

StepBackAction::StepBackAction(Dreidel* dreidle): dreidel(dreidel) {
}

int StepBackAction::execAction(float delta) {
	return 0;
}
