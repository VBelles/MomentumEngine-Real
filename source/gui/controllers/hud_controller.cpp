#include "mcv_platform.h"
#include "hud_controller.h"
#include "gui/gui_widget.h"
#include "components/player/comp_player_model.h"
#include "components/player/comp_power_gauge.h"

namespace GUI {
	CHudController::CHudController() {
		json& j = loadJson(hudVariablesFile);
		for (json& jData : j) {
			EngineGUI.getVariables().setVariant(jData);
		}
		chrysalisWidget = EngineGUI.getWidget(chrysalisWidgetName, true);
		assert(chrysalisWidget);
	}

	void CHudController::update(float delta) {
		if (chrysalisVisibleTime >= 0.f
			&& chrysalisWidget->getParams()->_visible && timer.elapsed() > chrysalisVisibleTime) {
			chrysalisWidget->getParams()->_visible = false;
		}

		TCompPlayerModel* playermodel = ((CEntity*)getEntityByName(PLAYER_NAME))->get<TCompPlayerModel>();
		TCompPowerGauge* powerGauge = playermodel->getPowerGauge();

		EngineGUI.getVariables().getVariant("hp")->setInt(playermodel->getHp());
		EngineGUI.getVariables().getVariant("max_hp")->setInt(playermodel->getMaxHp());
		EngineGUI.getVariables().getVariant("hp_progress")->setFloat(playermodel->getHp() / playermodel->getMaxHp());

		int coinBars = playermodel->getNumberOfCoins() * playermodel->getPowerPerCoin() / powerGauge->getMaxPower();
		int coinsPerBar = powerGauge->getMaxPower() / playermodel->getPowerPerCoin();
		int maxCoinBars = playermodel->getMaxNumberOfCoins() / coinsPerBar;
		float coinProgress = 1.f;
		if (coinBars != maxCoinBars) {
			coinProgress = playermodel->getNumberOfCoins() / (float)coinsPerBar - coinBars;
		}
		//EngineGUI.getVariables().getVariant("coin_full_foreground")->setFloat(coinBars);
		EngineGUI.getVariables().getVariant("coin_bars")->setInt(coinBars);
		EngineGUI.getVariables().getVariant("max_coin_bars")->setInt(maxCoinBars - 1);
		EngineGUI.getVariables().getVariant("coin_progress")->setFloat(coinProgress);

		EngineGUI.getVariables().getVariant("chrysalis")->setInt(playermodel->getCollectableManager()->getNumberOfChrysalis());
		EngineGUI.getVariables().getVariant("chrysalis1")->setInt(playermodel->getCollectableManager()->getNumberOfChrysalis() / 10);
		EngineGUI.getVariables().getVariant("chrysalis2")->setInt(playermodel->getCollectableManager()->getNumberOfChrysalis() % 10);
		EngineGUI.getVariables().getVariant("max_chrysalis")->setInt(EngineUniques.getNumUniqueElement(ElementType::CHRYSALIS));

		EngineGUI.getVariables().getVariant("life_pieces")->setInt(playermodel->getCollectableManager()->getNumberOfLifePieces());
		EngineGUI.getVariables().getVariant("max_life_pieces")->setInt(EngineUniques.getNumUniqueElement(ElementType::LIFEPIECE));

		EngineGUI.getVariables().getVariant("power_ups")->setInt(playermodel->getCollectableManager()->numberOfPowerUpsTaken);
		EngineGUI.getVariables().getVariant("max_power_ups")->setInt(EngineUniques.getNumUniqueElement(ElementType::POWERUP));

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

	void CHudController::showChrysalis() {
		chrysalisWidget->getParams()->_visible = true;
		chrysalisVisibleTime = -1.f;
	}

	void CHudController::showChrysalis(float time) {
		chrysalisWidget->getParams()->_visible = true;
		if (time > 0.f && chrysalisVisibleTime - timer.elapsed() < time) {
			chrysalisVisibleTime = time;
			timer.reset();
		}
	}

	void CHudController::hideChrysalis() {
		chrysalisWidget->getParams()->_visible = false;
	}
}
