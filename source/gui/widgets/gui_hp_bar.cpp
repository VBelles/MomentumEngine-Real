#include "mcv_platform.h"
#include "gui_hp_bar.h"
#include "gui/widgets/gui_image.h"

using namespace GUI;

void CHpBar::render() {
	int maxHp = EngineGUI.getVariables().getVariant("max_hp")->getInt();
	for (int i = 0; i < maxHp; i++) {
		hpImage->_params._position
	}
}

