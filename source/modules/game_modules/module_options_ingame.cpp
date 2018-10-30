#include "mcv_platform.h"
#include "module_options_ingame.h"
#include "gui/gui_parser.h"
#include "modules/system_modules/sound/comp_sound.h"
#include "modules/system_modules/sound/music_player.h"
#include "modules/system_modules/scripting/scripting_player.h"
#include "modules/game_modules/game/module_game.h"

#include <fstream>

bool CModuleOptionsIngame::start() {
	GUI::CParser parser;
	parser.parseFile(menuFile);

	jOptions = loadJson(optionsFile);

	controller = new GUI::COptionsMenuController();
	controller->registerOption("master_volume", jOptions["sound"]["master_volume"]);
	controller->registerOption("music_volume", jOptions["sound"]["music_volume"]);
	controller->registerOption("sound_volume", jOptions["sound"]["sound_volume"]);
	controller->registerOption("camera_speed", jOptions["camera"]["camera_speed"]);
	controller->registerOption("camera_speed_pad", jOptions["camera"]["camera_speed_pad"]);
	controller->registerOption("invert_x_axis", jOptions["camera"]["invert_x_axis"]);
	controller->registerOption("invert_y_axis", jOptions["camera"]["invert_y_axis"]);
	controller->registerOption("vsync", jOptions["screen"]["vsync"]);
	controller->setCurrentOption(0);

	return true;
}

bool CModuleOptionsIngame::stop() {
	jOptions["camera"]["invert_x_axis"] = controller->getOptionValue("invert_x_axis");
	jOptions["camera"]["invert_y_axis"] = controller->getOptionValue("invert_y_axis");
	jOptions["screen"]["vsync"] = controller->getOptionValue("vsync");
	jOptions["camera"]["camera_speed"] = controller->getOptionValue("camera_speed");
	jOptions["camera"]["camera_speed_pad"] = controller->getOptionValue("camera_speed_pad");
	jOptions["sound"]["master_volume"] = controller->getOptionValue("master_volume");
	jOptions["sound"]["music_volume"] = controller->getOptionValue("music_volume");
	jOptions["sound"]["sound_volume"] = controller->getOptionValue("sound_volume");

	std::string string = jOptions.dump(1, '\t');
	std::ofstream myfile;
	myfile.open(optionsFile);
	myfile << string;
	myfile.close();

	CApp::get().readConfig();

	Engine.getGUI().unregisterController(controller);
	safeDelete(controller);
	Engine.getGUI().unregisterWidget("options_ingame", true);
	return true;
}

void CModuleOptionsIngame::update(float delta) {
	if (active && EngineInput["menu_back"].getsReleased()) {
		activate(false);
		CApp::get().setResetMouse(true);
	}
}

void CModuleOptionsIngame::render() {}

void CModuleOptionsIngame::activate() {
	activate(true);
}

void CModuleOptionsIngame::activate(bool newActive) {
	active = newActive;
	TCompSound* sound = static_cast<CEntity*>(getEntityByName(SOUND_PLAYER))->get<TCompSound>();

	if (active) {
		controller->setCurrentOption(0);
		Engine.getGUI().registerController(controller);
		Engine.getGUI().activateWidget("options_ingame");
		ScriptingPlayer::givePlayerControl(); //Necesario ya que se fuerza salir del debug y puede no tener el control
		CApp::get().setDebugMode(false);
		CApp::get().setResetMouse(false);
		cb_globals.game_paused = 1;
		sound->stop();
		EngineSound.getMusicPlayer()->setPauseMenu(true);
	}
	else {
		Engine.getGUI().deactivateWidget("options_ingame");
		Engine.getGUI().unregisterController(controller);
		cb_globals.game_paused = 0;
		EngineSound.getMusicPlayer()->setPauseMenu(false);
		EngineSound.emitEvent(SOUND_MENU_BACK);
		sound->play("ambient_day");

		jOptions["camera"]["invert_x_axis"] = controller->getOptionValue("invert_x_axis");
		jOptions["camera"]["invert_y_axis"] = controller->getOptionValue("invert_y_axis");
		jOptions["screen"]["vsync"] = controller->getOptionValue("vsync");
		jOptions["camera"]["camera_speed"] = controller->getOptionValue("camera_speed");
		jOptions["camera"]["camera_speed_pad"] = controller->getOptionValue("camera_speed_pad");
		jOptions["sound"]["master_volume"] = controller->getOptionValue("master_volume");
		jOptions["sound"]["music_volume"] = controller->getOptionValue("music_volume");
		jOptions["sound"]["sound_volume"] = controller->getOptionValue("sound_volume");

		std::string string = jOptions.dump(1, '\t');
		std::ofstream myfile;
		myfile.open(optionsFile);
		myfile << string;
		myfile.close();

		CApp::get().readConfig();
	}

	EngineGame->showHUD(!active);
	EngineGUI.setDialogActive(!active);
	EngineRender.setActive(!active);
	EngineScripting.setActive(!active);
	Engine.getEntities().setActive(!active);
	EngineParticles.setPaused(active);
	EngineSound.setPaused(active);
	CTimerFrames::get().setPaused(active);
	CGameState* currentGamestate = EngineModules.getCurrentGameState();
	for (auto& module : *currentGamestate) {
		if (module != this) {
			module->setActive(!active);
		}
	}
}
