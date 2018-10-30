#include "mcv_platform.h"
#include "module_credits.h"
#include "gui/gui_parser.h"
#include "modules/system_modules/sound/comp_sound.h"
#include "modules/system_modules/sound/music_player.h"
#include "modules/system_modules/scripting/scripting_player.h"

bool CModuleCredits::start() {
	GUI::CParser parser;
	parser.parseFile(file);

	return true;
}

bool CModuleCredits::stop() {
	EngineGUI.unregisterWidget("credits");
	return true;
}

void CModuleCredits::update(float delta) {
	if (active && EngineInput["menu_back"].getsReleased()) {
		EngineGUI.deactivateWidget("credits");
		active = false;

		cb_globals.game_paused = 0;
		EngineSound.getMusicPlayer()->setPauseMenu(false);
		EngineSound.emitEvent(SOUND_MENU_BACK);
		TCompSound* sound = static_cast<CEntity*>(getEntityByName(SOUND_PLAYER))->get<TCompSound>();
		sound->play("ambient_day");
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
	else if (active && EngineInput["menu_accept"].getsReleased()) {
		GUI::CWidget* wdgt = EngineGUI.getWidget("credits")->getChild("controller");
		wdgt->getParams()->_visible = !wdgt->getParams()->_visible;
	}
}

void CModuleCredits::render() {}

void CModuleCredits::activate() {
	active = true;
	EngineGUI.activateWidget("credits");

	TCompSound* sound = static_cast<CEntity*>(getEntityByName(SOUND_PLAYER))->get<TCompSound>();
	ScriptingPlayer::givePlayerControl(); //Necesario ya que se fuerza salir del debug y puede no tener el control
	CApp::get().setDebugMode(false);
	cb_globals.game_paused = 1;
	sound->stop();
	EngineSound.getMusicPlayer()->setPauseMenu(true);
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
