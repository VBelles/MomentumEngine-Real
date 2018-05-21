#include "mcv_platform.h"
#include "hud_controller.h"
#include "gui/widgets/gui_button.h"

namespace GUI {
	CHudController::CHudController() {
		json& j = loadJson("data/gui/hud_variables.json");
		for (json& jData : j) {
			EngineGUI.getVariables().setVariant(jData);
		}
	}
	void CHudController::update(float delta) {

	}


}
