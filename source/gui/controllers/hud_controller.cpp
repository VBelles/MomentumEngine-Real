#include "mcv_platform.h"
#include "hud_controller.h"
#include "gui/widgets/gui_button.h"
#include "gui/widgets/gui_text.h"

namespace GUI {
	CHudController::CHudController() {
		json& j = loadJson("data/gui/hud_variables.json");
		for (json& jData : j) {
			EngineGUI.getVariables().setVariant(jData);
		}
	}
	void CHudController::update(float delta) {

	}

	void CHudController::updateHp(float hp, float maxHp) {
		EngineGUI.getVariables().getVariant("hp")->setString(std::to_string((int)hp));
		EngineGUI.getVariables().getVariant("hp_progress")->setFloat(hp / maxHp);
		updateText();

	}
	void CHudController::updatePower(float power, float maxPower) {
		EngineGUI.getVariables().getVariant("power_progress")->setFloat(power / maxPower);
		updateText();
	}

	void CHudController::updateText() {
		auto widgets = EngineGUI.getWidget("hud")->getChildren(true);
		for (auto widget : widgets) {
			CText* text = static_cast<CText*>(widget);
			if (text) {
				text->updateTemplate();
			}
		}
	}

}
