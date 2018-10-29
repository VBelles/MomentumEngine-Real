#include "mcv_platform.h"
#include "comp_show_dialog.h"
//#include "gui/widgets/gui_dialog.h"

DECL_OBJ_MANAGER("show_dialog", TCompShowDialog);

void TCompShowDialog::debugInMenu() {
}

void TCompShowDialog::registerMsgs() {
	DECL_MSG(TCompShowDialog, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompShowDialog, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompShowDialog, TMsgTriggerExit, onTriggerExit);
	DECL_MSG(TCompShowDialog, TMsgColliderDestroyed, onColliderDestroyed);
}

void TCompShowDialog::load(const json & j, TEntityParseContext & ctx) {
	timeToShowMessage = j.value("show_time", timeToShowMessage);
	timeToRefreshMessage = j.value("refresh_time", timeToRefreshMessage);
	timeToAllowCancel = j.value("cancel_time", timeToAllowCancel);
	fontSize = j.value("font_size", fontSize);
	if (j.count("dialogs")) {
		auto& dialogs = j["dialogs"];
		assert(dialogs.is_array());
		for (std::string dialog : dialogs) {
			dialogsToShow.push_back(dialog);
		}
	}
}

void TCompShowDialog::update(float delta){
	if (!isPlayerInside) return;
	if (!firstMessageShown) {
		if (showMessageTimer.elapsed() >= timeToShowMessage) {
			isCancelable = true;
			EngineGUI.showDialog(dialogsToShow[dialogIndex], fontSize, isCancelable);
			firstMessageShown = true;
			dialogIndex = (dialogIndex + 1) % dialogsToShow.size();
			cancelTimer.reset();
			if (dialogIndex == 0) {
				lastMessageReached = true;
			}
		}
	}
	else {
		//alargar estos tiempos?
		if (!isCancelable && cancelTimer.elapsed() >= timeToAllowCancel) {
			isCancelable = true;
			EngineGUI.setCancelableWithButton(isCancelable);
		}
		if (!EngineGUI.isDialogActive()) {
			if (lastMessageReached) {
				refreshMessageTimer.reset();
				lastMessageReached = false;
			}
			if(dialogIndex == 0){
				//time to refresh
				if (EngineInput["previous_dialog"].getsPressed() || refreshMessageTimer.elapsed() >= timeToRefreshMessage) {
					isCancelable = true;
					EngineGUI.showDialog(dialogsToShow[dialogIndex], fontSize, isCancelable);
					dialogIndex = (dialogIndex + 1) % dialogsToShow.size();
					cancelTimer.reset();
					if (dialogIndex == 0) {
						lastMessageReached = true;
					}
				}
			}
			else {
				//show next dialog
				isCancelable = true;
				EngineGUI.showDialog(dialogsToShow[dialogIndex], fontSize, isCancelable);
				dialogIndex = (dialogIndex + 1) % dialogsToShow.size();
				cancelTimer.reset();
				if (dialogIndex == 0) {
					lastMessageReached = true;
				}
			}
		}
		else {
			if (EngineInput["previous_dialog"].getsPressed()) {
				if (dialogIndex == 0 && lastMessageReached) {
					dialogIndex = dialogsToShow.size() - 2;
					if (dialogIndex < 0) dialogIndex = 0;
					EngineGUI.showDialog(dialogsToShow[dialogIndex], fontSize, isCancelable);
					isCancelable = true;
					dialogIndex = (dialogIndex + 1) % dialogsToShow.size();
					cancelTimer.reset();
					if (dialogIndex == 0) {
						lastMessageReached = true;
					}
				}else if(dialogIndex > 0){
					dialogIndex = dialogIndex - 2;
					if (dialogIndex < 0) dialogIndex = 0;
					EngineGUI.showDialog(dialogsToShow[dialogIndex], fontSize, isCancelable);
					isCancelable = true;
					dialogIndex = (dialogIndex + 1) % dialogsToShow.size();
					cancelTimer.reset();
				}
			}
		}
	}

}

void TCompShowDialog::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
	//dialogWidget = (CDialog*)EngineGUI.getWidget(widgetName);
	std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
	UniqueElement* uniqueEvent = EngineUniques.getUniqueEvent(name);
	if (uniqueEvent && uniqueEvent->done) {
		((TCompCollider*)get<TCompCollider>())->destroy();
	}
}

void TCompShowDialog::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity* entity = msg.h_other_entity;
	if (PLAYER_NAME == entity->getName()) {
		//timers a 0, player inside
		cancelTimer.reset();
		showMessageTimer.reset();
		refreshMessageTimer.reset();
		isCancelable = false;
		firstMessageShown = false;
		isPlayerInside = true;
		dialogIndex = 0;
	}
}

void TCompShowDialog::onTriggerExit(const TMsgTriggerExit & msg) {
	CEntity* entity = msg.h_other_entity;
	if (PLAYER_NAME == entity->getName()) {
		EngineGUI.hideDialog();
		isPlayerInside = false;
	}
}

void TCompShowDialog::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	CHandle(this).getOwner().destroy();
}