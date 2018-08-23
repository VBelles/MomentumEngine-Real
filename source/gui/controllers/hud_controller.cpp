#include "mcv_platform.h"
#include "hud_controller.h"
#include "gui/widgets/gui_button.h"
#include "gui/widgets/gui_text.h"
#include "components/player/comp_player_model.h"
#include "components/player/comp_power_gauge.h"

namespace GUI {
	CHudController::CHudController() {
		json& j = loadJson("data/gui/hud_variables.json");
		for (json& jData : j) {
			EngineGUI.getVariables().setVariant(jData);
		}
	}

	void CHudController::update(float delta) {
		TCompPlayerModel* playermodel = ((CEntity*)getEntityByName(PLAYER_NAME))->get<TCompPlayerModel>();
		TCompPowerGauge* powerGauge = playermodel->getPowerGauge();

		EngineGUI.getVariables().getVariant("hp")->setInt(playermodel->getHp());
		EngineGUI.getVariables().getVariant("hp_progress")->setFloat(playermodel->getHp() / playermodel->getMaxHp());

		int coinBars = playermodel->getNumberOfCoins() * playermodel->getPowerPerCoin() / powerGauge->getMaxPower();
		int coinsPerBar = powerGauge->getMaxPower() / playermodel->getPowerPerCoin();
		float coinProgress = playermodel->getNumberOfCoins() / (float)coinsPerBar - coinBars;
		EngineGUI.getVariables().getVariant("coin_full_foreground")->setFloat(coinBars);
		EngineGUI.getVariables().getVariant("coin_bars")->setInt(coinBars);
		EngineGUI.getVariables().getVariant("coin_progress")->setFloat(coinProgress);

		//EngineGUI.getVariables().getVariant("power_progress")->setFloat(powerGauge->getBarPercentage());
		if (powerGauge->getPowerLevel() == 1) {
			EngineGUI.getVariables().getVariant("power_progress_level1")->setFloat(powerGauge->getPowerLevelPercentage());
			EngineGUI.getVariables().getVariant("power_progress_level2")->setFloat(0);
			EngineGUI.getVariables().getVariant("power_progress_level3")->setFloat(0);
		}
		else if (playermodel->getPowerGauge()->getPowerLevel() == 2) {
			EngineGUI.getVariables().getVariant("power_progress_level1")->setFloat(1);
			EngineGUI.getVariables().getVariant("power_progress_level2")->setFloat(powerGauge->getPowerLevelPercentage());
			EngineGUI.getVariables().getVariant("power_progress_level3")->setFloat(0);
		}
		else if (playermodel->getPowerGauge()->getPowerLevel() == 3) {
			EngineGUI.getVariables().getVariant("power_progress_level1")->setFloat(1);
			EngineGUI.getVariables().getVariant("power_progress_level2")->setFloat(1);
			EngineGUI.getVariables().getVariant("power_progress_level3")->setFloat(powerGauge->getPowerLevelPercentage());
		}
	}
}
