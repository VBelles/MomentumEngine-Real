#include "mcv_platform.h"
#include "module_pause.h"
#include "gui/gui_parser.h"
#include "modules/system_modules/scripting/scripting_player.h"
#include "modules/system_modules/module_gui.h"
#include "modules/game_modules/game/module_game.h"
#include "modules/system_modules/sound/comp_music.h"
#include "modules/system_modules/sound/comp_sound.h"
#include "modules/system_modules/sound/music_player.h"
#include "modules/game_modules/module_options_ingame.h"
#include "modules/game_modules/module_controls_help.h"

bool CModulePause::start() {
	pause = false;

	GUI::CParser parser;
	parser.parseFile("data/gui/test_pause_menu.json");

	auto resumeGameCB = [&] {
		onPausePressed();
		CApp::get().setResetMouse(!pause);
	};
	auto optionsCB = [&] {
		onPausePressed();
		((CModuleOptionsIngame*)EngineModules.getModule("options_ingame"))->activate();
	};
	auto controlsCB = [&] {
		onPausePressed();
		((CModuleControlsHelp*)EngineModules.getModule("controls_help"))->activate();
	};
	auto mainMenuCB = [&]() {
		onPausePressed();
		EngineGUI.hideDialog();
		EngineModules.changeGameState("main_menu", true);
		EngineSound.getMusicPlayer()->setCurrentSong(CMusicPlayer::Song::INTRO);
	};
	auto exitCB = []() {
		CApp::get().stopMainLoop = true;
	};

	controller = new GUI::CMainMenuController();
	controller->registerOption("resume_game", resumeGameCB);
	controller->registerOption("options", optionsCB);
	controller->registerOption("controls", controlsCB);
	controller->registerOption("main_menu", mainMenuCB);
	controller->registerOption("exit_game", exitCB);
	blocked = false;

	return true;
}

bool CModulePause::stop() {
	Engine.getGUI().unregisterController(controller);
	safeDelete(controller);
	Engine.getGUI().unregisterWidget("test_pause_menu", true);
	blocked = false;
	return true;
}

void CModulePause::update(float delta) {
	if (!blocked && EngineInput["pause"].getsPressed()
		|| pause && EngineInput["menu_back"].getsPressed()) {
		onPausePressed();
		CApp::get().setResetMouse(!pause);
	}
}

void CModulePause::onPausePressed() {
	pause = !pause;
	//TCompMusic* music = static_cast<CEntity*>(getEntityByName(MUSIC_PLAYER))->get<TCompMusic>();
	TCompSound* sound = static_cast<CEntity*>(getEntityByName(SOUND_PLAYER))->get<TCompSound>();

	if (pause) {
		EngineGame->showChrysalis(0.f);
		Engine.getGUI().registerController(controller);
		controller->setCurrentOption(0);
		Engine.getGUI().activateWidget("test_pause_menu");
		ScriptingPlayer::givePlayerControl(); //Necesario ya que se fuerza salir del debug y puede no tener el control
		CApp::get().setDebugMode(false);
		cb_globals.game_paused = 1;
		sound->stop();
		EngineSound.getMusicPlayer()->setPauseMenu(true);
	}
	else {
		Engine.getGUI().deactivateWidget("test_pause_menu");
		Engine.getGUI().unregisterController(controller);
		cb_globals.game_paused = 0;
		EngineSound.getMusicPlayer()->setPauseMenu(false);
		EngineSound.emitEvent(SOUND_MENU_BACK);
		sound->play("ambient_day");
	}

	EngineRender.setActive(!pause);
	EngineScripting.setActive(!pause);
	Engine.getEntities().setActive(!pause);
	EngineParticles.setPaused(pause);
	EngineSound.setPaused(pause);
	CTimerFrames::get().setPaused(pause);
	CGameState* currentGamestate = EngineModules.getCurrentGameState();
	for (auto& module : *currentGamestate) {
		if (module != this) {
			module->setActive(!pause);
		}
	}
}

void CModulePause::setBlocked(bool blocked) {
	this->blocked = blocked;
}

bool CModulePause::isBlocked() {
	return blocked;
}

void CModulePause::render() {}
