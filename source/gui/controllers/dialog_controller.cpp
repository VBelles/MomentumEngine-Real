#include "mcv_platform.h"
#include "dialog_controller.h"
#include "gui/gui_parser.h"

namespace GUI {
	CDialogController::CDialogController() {
		GUI::CParser parser;
		parser.parseFile("data/gui/dialog_box.json");
		dialogWidget = (CDialog*)EngineGUI.getWidget(widgetName);
		dialogActive = false;
	}

	CDialogController::~CDialogController() {
		EngineGUI.unregisterWidget(widgetName);
	}

	void CDialogController::update(float delta) {
		if (EngineInput["menu_accept"].getsPressed()) {
			if (dialogActive) {
				if (dialogWidget->getCurrentFragment() < dialogWidget->getNumFragments() - 1) {
					dialogWidget->showNext();
				}
				else {
					hideDialog();
				}
			}
		}
	}

	void CDialogController::showDialog(const std::string& text, const int& fontSize) {
		hideDialog();
		dialogWidget->setText(text, fontSize);
		EngineGUI.activateWidget(widgetName);
		dialogActive = true;
	}

	void CDialogController::hideDialog() {
		EngineGUI.deactivateWidget(widgetName);
		dialogActive = false;
	}

	bool CDialogController::isActive() {
		return dialogActive;
	}
}
