#include "mcv_platform.h"
#include "module_credits.h"
#include "gui/gui_parser.h"
#include "modules/system_modules/sound/comp_sound.h"
#include "modules/system_modules/sound/music_player.h"
#include "modules/system_modules/scripting/scripting_player.h"

bool CModuleCredits::start() {
	GUI::CParser parser;
	parser.parseFile(file);
	EngineGUI.activateWidget("credits");

	Engine.setInputType(InputType::Menu);
	EngineSound.getMusicPlayer()->setCurrentSong(CMusicPlayer::Song::MENU);

	return true;
}

bool CModuleCredits::stop() {
	EngineGUI.unregisterWidget("credits");
	return true;
}

void CModuleCredits::update(float delta) {
	if (EngineInput["menu_back"].getsReleased() ||
		EngineInput["menu_accept"].getsReleased()) {
		EngineModules.changeGameState("main_menu");
	}
}

void CModuleCredits::render() {}
